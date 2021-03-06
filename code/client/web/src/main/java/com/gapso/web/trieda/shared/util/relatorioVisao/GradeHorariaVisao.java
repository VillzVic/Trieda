package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import com.extjs.gxt.ui.client.core.El;
import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.dnd.DragSource;
import com.extjs.gxt.ui.client.dnd.GridDropTarget;
import com.extjs.gxt.ui.client.event.BaseEvent;
import com.extjs.gxt.ui.client.event.DNDEvent;
import com.extjs.gxt.ui.client.event.DNDListener;
import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.GridEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Html;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.tips.QuickTip;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.dom.client.Style.Unit;
import com.google.gwt.i18n.client.NumberFormat;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public abstract class GradeHorariaVisao extends ContentPanel{
	protected Grid<LinhaDeCredito> grid;
	protected ListStore<LinhaDeCredito> store;
	protected boolean temInfoDeHorarios;
	protected ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas;
	protected Integer tamanhoLinhaGradeHorariaEmPixels;
	protected List<AtendimentoRelatorioDTO> atendimentoDTO;
	protected List<String> labelsDasLinhasDaGradeHoraria;
	protected List<String> horariosDeInicioDeAula;
	protected List<Boolean> horarioEhIntervalo;
	protected List<TrioDTO<String, Integer, Integer>> horariosDisponiveis;
	protected TurnoDTO turnoDTO;
	protected QuickTip quickTip;
	protected List<Long> disciplinasCores = new ArrayList<Long>();
	protected Map<Long, String> disciplinaIdToCSSStyleMap = new HashMap<Long, String>();
	protected CenarioDTO cenarioDTO;
	protected AulaDTO aulaDestaque;
	protected boolean gradeHorariaAlocacaoManual;
	protected Set<String> horarioInicioAula = new HashSet<String>();
	protected Set<String> horarioFimAula = new HashSet<String>();
	protected List<String> horariosEscritos = new ArrayList<String>();
	protected List<Integer> labelReduzidaTamanho = new ArrayList<Integer>();

	protected String emptyTextBeforeSearch = "Preencha o filtro acima";
	protected String emptyTextAfterSearch = "Não foi encontrado nenhuma Grade Horária para este filtro";
	
	public GradeHorariaVisao(CenarioDTO cenarioDTO, boolean gradeHorariaAlocacaoManual){
		super(new FitLayout());
		this.temInfoDeHorarios = true;
		this.mdcTemposAulaNumSemanasLetivas = ParDTO.create(1, false);
		this.tamanhoLinhaGradeHorariaEmPixels = 0;
		this.labelsDasLinhasDaGradeHoraria = new ArrayList<String>();
		this.horariosDeInicioDeAula = new ArrayList<String>();
		this.horarioEhIntervalo = new ArrayList<Boolean>();
		this.cenarioDTO = cenarioDTO;
		this.gradeHorariaAlocacaoManual = gradeHorariaAlocacaoManual;
		this.setHeaderVisible(false);
	}
	
	public GradeHorariaVisao(CenarioDTO cenarioDTO){
		super(new FitLayout());
		this.temInfoDeHorarios = true;
		this.mdcTemposAulaNumSemanasLetivas = ParDTO.create(1, false);
		this.tamanhoLinhaGradeHorariaEmPixels = 0;
		this.labelsDasLinhasDaGradeHoraria = new ArrayList<String>();
		this.horariosDeInicioDeAula = new ArrayList<String>();
		this.horarioEhIntervalo = new ArrayList<Boolean>();
		this.cenarioDTO = cenarioDTO;
		this.gradeHorariaAlocacaoManual = false;
		this.setHeaderVisible(false);
	}
	
	protected String getEmptyTextAfterSearch()
	{
		return emptyTextAfterSearch;
	}
	
	@Override
	protected void beforeRender(){
		super.beforeRender();

		this.grid = new Grid<LinhaDeCredito>(getListStore(), new ColumnModel(getColumnList(null)));
		this.grid.setTrackMouseOver(false);
		this.grid.setStyleName("GradeHorariaGrid VisaoSala");
		
		this.grid.addListener(Events.BeforeSelect, new Listener<GridEvent<LinhaDeCredito>>(){
			@Override
			public void handleEvent(GridEvent<LinhaDeCredito> be){
				be.setCancelled(true);
			}
		});

		this.grid.getView().setEmptyText(this.emptyTextBeforeSearch);
		this.quickTip = new QuickTip(this.grid);
		this.quickTip.getToolTipConfig().setDismissDelay(0);
		add(this.grid);

		GridDropTarget target = new GridDropTarget(this.grid){
			@Override
			protected void onDragDrop(DNDEvent event){}
		};

		target.addDNDListener(new DNDListener(){
			@Override
			public void dragMove(DNDEvent e){
				int linha = grid.getView().findRowIndex(e.getDragEvent().getTarget());
				int coluna = grid.getView().findCellIndex(e.getDragEvent().getTarget(), null);
				
				boolean value = linha < 0 || coluna < 1;
				e.setCancelled(value);
				e.getStatus().setStatus(!value);

				return;
			}
		});

		requestAtendimentos();
		checkRegistered();
		
	}
	
	public void addGridListener(EventType eventType, Listener<? extends BaseEvent> listener)
	{
		if (this.grid != null && this.grid.getListeners(eventType).isEmpty())
		{
			this.grid.addListener(eventType, listener);
		}
		else
		{
		}
	}
	
	public abstract RelatorioVisaoFiltro getFiltro();
	public abstract void setFiltro(RelatorioVisaoFiltro filtro);
	public abstract void requestAtendimentos();
	public abstract void checkRegistered();
	
	
	
	protected AsyncCallback<AtendimentoServiceRelatorioResponse> getCallback(){
		return (AsyncCallback<AtendimentoServiceRelatorioResponse>) new AsyncCallback<AtendimentoServiceRelatorioResponse>(){
			@Override
			public void onFailure(Throwable caught) {
				if (caught instanceof TriedaException) {
					MessageBox.alert("ERRO!",caught.getMessage(), null);
				}
				else {
					MessageBox.alert("ERRO!","Não foi possível carregar a grade de horários", null);
				}
			}

			@Override
			public void onSuccess(AtendimentoServiceRelatorioResponse result){
				grid.getView().setEmptyText(getEmptyTextAfterSearch());
				labelsDasLinhasDaGradeHoraria.clear();
				horariosDeInicioDeAula.clear();
				horarioEhIntervalo.clear();
				horariosEscritos.clear();
				labelReduzidaTamanho.clear();
				horarioInicioAula.clear();
				horarioFimAula.clear();
				atendimentoDTO = result.getAtendimentosDTO();
				horariosDisponiveis = result.getHorariosDisponiveis();
				if(!atendimentoDTO.isEmpty() || gradeHorariaAlocacaoManual){
					if (atendimentoDTO.isEmpty())
					{
						temInfoDeHorarios = true;
					}
					else
					{
						temInfoDeHorarios = (atendimentoDTO.iterator().next().getHorarioAulaId() != null);
					}
					mdcTemposAulaNumSemanasLetivas = result.getMdcTemposAulaNumSemanasLetivas();
					if (temInfoDeHorarios) {
						TrioDTO<List<String>,List<String>, List<Boolean>> trioDTO = GradeHoraria.processaLabelsDasLinhasDaGradeHoraria(result.getLabelsDasLinhasDaGradeHoraria(),result.getHorariosDeInicioDeAula(),result.getHorariosDeFimDeAula());
						labelsDasLinhasDaGradeHoraria.addAll(trioDTO.getPrimeiro());
						horariosDeInicioDeAula.addAll(trioDTO.getSegundo());//horariosDeInicioDeAula.addAll(result.getHorariosDeInicioDeAula());
						horarioEhIntervalo.addAll(trioDTO.getTerceiro());			
						geraLinhasReduzidas();
					} else {
						labelsDasLinhasDaGradeHoraria.addAll(result.getLabelsDasLinhasDaGradeHoraria());
						for (int i = 0; i < result.getLabelsDasLinhasDaGradeHoraria().size(); i++) {
							horarioEhIntervalo.add(false);
						}
					}
					tamanhoLinhaGradeHorariaEmPixels = (int)(GradeHoraria.PIXELS_POR_MINUTO /** mdcTemposAulaNumSemanasLetivas.getPrimeiro()*/);
					if(tamanhoLinhaGradeHorariaEmPixels < 20){
						tamanhoLinhaGradeHorariaEmPixels = 20;
					}
					
					List<ColumnConfig> columns = getColumnList(result.getQtdColunasPorDiaSemana());
					preencheCoresElevaEducacao();//preencheCores();
					grid.reconfigure(getListStore(), new ColumnModel(columns));
					int totalLinhas = horariosEscritos.size();
					for(int row = 0; row < totalLinhas; row++){
						if(isHorarioIntervalo(horariosEscritos.get(row))/* && !mdcTemposAulaNumSemanasLetivas.getSegundo()*/) {
							grid.getView().getRow(row).getStyle().setHeight(10, Unit.PX);
						}
						else {
							grid.getView().getRow(row).getStyle().setHeight(tamanhoLinhaGradeHorariaEmPixels * (calculaTamanhoLabelReduzida(horariosEscritos.get(row))/15) - 2, Unit.PX);
						}
					}
				}
				else{
					mdcTemposAulaNumSemanasLetivas = ParDTO.create(1, false);
					tamanhoLinhaGradeHorariaEmPixels = 0;
					grid.reconfigure(getListStore(), new ColumnModel(getColumnList(null)));
				}
				grid.unmask();
			}

			private void geraLinhasReduzidas() {
				String pattern = "00";
				//Pegando horarios de inicio e fim das aulas para minimizar numero de linhas
				if (atendimentoDTO != null)
				{
					for(AtendimentoRelatorioDTO aula : atendimentoDTO){
						String[] horarioInicialArray = aula.getHorarioAulaString().split(":");
						int horarioInicialHoras = Integer.parseInt(horarioInicialArray[0]);
						int horarioInicialMinutos = Integer.parseInt(horarioInicialArray[1]);
						
						horarioInicioAula.add(aula.getHorarioAulaString());
						for (int i = 1; i <= aula.getTotalCreditos() ; i++)
						{
							int horarioFinalHoras = horarioInicialHoras;
							int horarioFinalMinutos = horarioInicialMinutos + (aula.getDuracaoDeUmaAulaEmMinutos() * i);
							while (horarioFinalMinutos >= 60)
							{
								horarioFinalHoras++;
								horarioFinalMinutos -= 60;
							}
							horarioFimAula.add(NumberFormat.getFormat(pattern).format(horarioFinalHoras) + ":" + NumberFormat.getFormat(pattern).format(horarioFinalMinutos));
						}
					}
				}

				if (horarioInicioAula.isEmpty() || gradeHorariaAlocacaoManual)
				{
					horariosEscritos.addAll(labelsDasLinhasDaGradeHoraria);
				}
				else
				{
					String horarioASerEscrito = "";
					if (!labelsDasLinhasDaGradeHoraria.isEmpty())
						horarioASerEscrito = labelsDasLinhasDaGradeHoraria.get(0).substring(0, 5);
					for (int i = 1; i < labelsDasLinhasDaGradeHoraria.size(); i++) {
						String horarioEmProcessamento = labelsDasLinhasDaGradeHoraria.get(i).substring(0, 5);
						if ((horarioInicioAula.contains(horarioEmProcessamento) || 
							 horarioFimAula.contains(horarioEmProcessamento))
							/*&& !isHorarioIntervalo(horarioEmProcessamento)*/)
						{
							horarioASerEscrito += " / " + horarioEmProcessamento;
							labelReduzidaTamanho.add(calculaTamanhoLabelReduzida(horarioASerEscrito));
							horariosEscritos.add(horarioASerEscrito);
							horarioASerEscrito = horarioEmProcessamento;
						}
					}
					if (!labelsDasLinhasDaGradeHoraria.isEmpty())
					{
						horarioASerEscrito += " / " + labelsDasLinhasDaGradeHoraria.get(labelsDasLinhasDaGradeHoraria.size()-1).substring(8, 13);
						labelReduzidaTamanho.add(calculaTamanhoLabelReduzida(horarioASerEscrito));
						horariosEscritos.add(horarioASerEscrito);
					}
				}
			}
		};
	}
	
	public ListStore<LinhaDeCredito> getListStore(){
		if(this.store == null) this.store = new ListStore<LinhaDeCredito>();
		else this.store.removeAll();

		for (int i = 0; i < horariosEscritos.size(); i++) {
			boolean isIntervalo = false;
			for (int j = 0; j < labelsDasLinhasDaGradeHoraria.size(); j++)
			{
				if (labelsDasLinhasDaGradeHoraria.get(j).contains(horariosEscritos.get(i)))
					isIntervalo = horarioEhIntervalo.get(j);
			}
			if(isIntervalo) {
				this.store.add(new LinhaDeCredito("", i));
			}
			else {
				this.store.add(new LinhaDeCredito(horariosEscritos.get(i), i));
			}
		}
		
		return this.store;
	}
	
	@Override
	protected void onRender(Element parent, int pos){
		super.onRender(parent, pos);
	}

	public List<ColumnConfig> getColumnList(List<Integer> l){
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();

		if(temInfoDeHorarios) {
			addColumn(list, "display", "Horários");
		} else {
			addColumn(list, "display", "Carga Horária (Min)");
		}

		addColumn(list, "segunda", "Segunda");
		addColumn(list, "terca", "Terça");
		addColumn(list, "quarta", "Quarta");
		addColumn(list, "quinta", "Quinta");
		addColumn(list, "sexta", "Sexta");
		addColumn(list, "sabado", "Sábado");
		addColumn(list, "domingo", "Domingo");

		return list;
	}
	
	protected void addColumn(List< ColumnConfig > list, String id, String name){
		GridCellRenderer< LinhaDeCredito > change = getGridCellRenderer();
		int width = (name.equals("")) ? 0 : getWidth(id);

		ColumnConfig column = new ColumnConfig(id, name, width);
		
		column.setRenderer(change);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);

		list.add(column);
	}
	
	protected int getWidth(String semana){
		return 100;
	}
	
	protected  GridCellRenderer<LinhaDeCredito> getGridCellRenderer(){
		return new GridCellRenderer<LinhaDeCredito>(){
			public Html render(LinhaDeCredito model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<LinhaDeCredito> store, Grid<LinhaDeCredito> grid) {
				if (colIndex == 0) {
					Html html = new Html(model.getDisplay());
					html.setStyleAttribute("line-height", tamanhoLinhaGradeHorariaEmPixels * (calculaTamanhoLabelReduzida(horariosEscritos.get(rowIndex))/15) - 2 + "px");
					html.setStyleAttribute("margin-top", "-4px");
					return html;
				}

				return content(model,rowIndex,colIndex);
			}

			private Html content(LinhaDeCredito model, int rowIndex, int colIndex){
				if((atendimentoDTO == null || atendimentoDTO.isEmpty()) && !gradeHorariaAlocacaoManual) return new Html("");

				int semana = getSemana(colIndex);

				AtendimentoRelatorioDTO aulaDTO = null;
				if (temInfoDeHorarios) {
					aulaDTO = getAulaPorHorario(model.getLinhaNaGradeHoraria(),semana);
				} else {
					aulaDTO = getProximaAula(rowIndex+1,semana);
				}
				//Verifica se a celula da grade horaria eh um horario indisponivel (somente para sala ou professor)
				if (horariosDisponiveis != null && !model.getDisplay().isEmpty())
				{
					boolean horarioVazio = true;
					for (TrioDTO<String, Integer, Integer> hoarioDisponivel : horariosDisponiveis)
					{
						if ( hoarioDisponivel.getSegundo().equals(semana+1)
								&& checkEntreHorarioDisponivel(hoarioDisponivel.getPrimeiro(), horariosEscritos.get(rowIndex).substring(0, 5), hoarioDisponivel.getTerceiro() ))
						{
							horarioVazio = false;
						}
					}
					if (horarioVazio)
					{
						Html html = new Html("");
						
						html.setStyleAttribute("height", tamanhoLinhaGradeHorariaEmPixels * (calculaTamanhoLabelReduzida(horariosEscritos.get(rowIndex))/15) + "px");
						html.setStyleAttribute("margin-top", "-5px");
						html.setStyleAttribute("background-color", "black");
						html.setStyleAttribute("opacity", "0.3");
						
						return html;
					}
				}
				if (aulaDTO == null) {
					Html html = new Html("");
					if (!model.getDisplay().isEmpty() && gradeHorariaAlocacaoManual)
					{
						html.setStyleAttribute("height", tamanhoLinhaGradeHorariaEmPixels * (calculaTamanhoLabelReduzida(horariosEscritos.get(rowIndex))/15) - 5 + "px");
						html.addStyleName("gradeVazia");
					}
					return html;
				}
				TrioDTO<String, String, String> htmlInfo = getHTMLInfo(aulaDTO);

				final String title = htmlInfo.getPrimeiro();
				final String contentToolTip = htmlInfo.getSegundo();
				final Html html = new Html(htmlInfo.getTerceiro()){
					@Override
					protected void onRender(Element target, int index){
						super.onRender(target, index);
						target.setAttribute("qtip", contentToolTip);
						target.setAttribute("qtitle", title);
						target.setAttribute("qwidth", "400px");
					}
				};
				html.addStyleName("horario");
				if (aulaDestaque != null)
				{
					if (aulaDTO.getHorarioAulaId().equals(aulaDestaque.getHorarioAulaId())
							&& aulaDTO.getDiaSemana().equals(aulaDestaque.getSemana()))
					{
						html.addStyleName("horarioDestaque");
					}
				}
				html.setStyleAttribute("top", calculaPosicaoAula(rowIndex) + "px");
				// calcula a quantidade de linhas, para cada crédito, que a aula em questão ocupa na grade horária
				int qtdLinhasNaGradeHorariaPorCreditoDaAula = getIndexHorario(getHorarioFinalAula(aulaDTO), horariosEscritos) - getIndexHorario(aulaDTO.getHorarioAulaString(), horariosEscritos);
				int height = 0;
				for (int i = rowIndex; i < rowIndex+qtdLinhasNaGradeHorariaPorCreditoDaAula; i++)
				{
					height += tamanhoLinhaGradeHorariaEmPixels * (calculaTamanhoLabelReduzida(horariosEscritos.get(i))/15);
				}
				html.setStyleAttribute("height", (height - 3)  + "px");
				if (!gradeHorariaAlocacaoManual && horariosDisponiveis == null)
				{
					html.addStyleName("s" + aulaDTO.getSemana()); // Posiciona na coluna ( dia semana )
				}
				html.addStyleName(getCssDisciplina(aulaDTO.getDisciplinaId()));

				new DragSource(html){
					@Override
					protected void onDragStart(DNDEvent event){
						event.setData(html);
						event.getStatus().update(El.fly(html.getElement()).cloneNode(true));
						quickTip.hide();
					}
				};

				return html;
			}
		};
	}
	
	// Recebe um horario disponivel + tempo de aula e checa se a linha na grade horaria esta disponivel
	// Eh preciso desse metodo para quando os horarios estao divididos por causa do MDC
	private boolean checkEntreHorarioDisponivel(String hoarioDisponivel, String horarioAula, Integer tempo) {
		String[] horarioInicialArray = hoarioDisponivel.split(":");
		int horarioInicialHoras = Integer.parseInt(horarioInicialArray[0]);
		int horarioInicialMinutos = Integer.parseInt(horarioInicialArray[1]);
		
		
		int horarioFinalHoras = Integer.parseInt(horarioInicialArray[0]);
		int horarioFinalMinutos = Integer.parseInt(horarioInicialArray[1]) + tempo;
		while (horarioFinalMinutos >= 60)
		{
			horarioFinalHoras++;
			horarioFinalMinutos -= 60;
		}
		
		String[] horarioLinhaArray = horarioAula.split(":");
		int horarioLinhaHoras = Integer.parseInt(horarioLinhaArray[0]);
		int horarioLinhaMinutos = Integer.parseInt(horarioLinhaArray[1]);
		return ((horarioLinhaHoras > horarioInicialHoras
				|| (horarioLinhaHoras == horarioInicialHoras && horarioLinhaMinutos >= horarioInicialMinutos))
				&&
				((horarioLinhaHoras < horarioFinalHoras) 
				|| (horarioLinhaHoras == horarioFinalHoras && horarioLinhaMinutos <= horarioFinalMinutos)));		
		
	}
	
	protected int getNumeroIntervalos(int rowIndex) {
		int numeroIntervalos = 0;
		if (mdcTemposAulaNumSemanasLetivas.getSegundo())
			return 0;	
		for(int i = 0; i<rowIndex; i++){
			if(isHorarioIntervalo(horariosEscritos.get(i))){
				numeroIntervalos++;
			}
		}
		return numeroIntervalos;
	}
	
	protected boolean isHorarioIntervalo(String horario) {
		if (horario.contains("/")) { // é um intervalo, isto é, hi / hf
			for (int i = 0; i<labelsDasLinhasDaGradeHoraria.size(); i++)
			{
				if (labelsDasLinhasDaGradeHoraria.get(i).contains(horario))
					return horarioEhIntervalo.get(i);
			}
		} else { // é apenas um horário
			for (int i = 0; i<horariosDeInicioDeAula.size(); i++)
			{
				if (horariosDeInicioDeAula.get(i).contains(horario))
					return horarioEhIntervalo.get(i);
			}
		}
		return false;
	}

	protected int getSemana(int colIndex){
		int semana = -1;
		if(colIndex > 0 || colIndex < 7) semana = colIndex++;
		
		return semana;
	}
	
	protected abstract TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO);
	
	protected AtendimentoRelatorioDTO getAulaPorHorario(int linhaGradeHoraria, int colunaGradeHoraria){
		if (gradeHorariaAlocacaoManual || horariosDisponiveis != null)
		{
			colunaGradeHoraria += 1;
		}
		if(this.atendimentoDTO != null){
			String labelHorario = horariosEscritos.get(linhaGradeHoraria).substring(0,5);
			for(AtendimentoRelatorioDTO aula : this.atendimentoDTO){
				if(aula.getSemana() == colunaGradeHoraria){
					if(labelHorario.contains(aula.getHorarioAulaString()))
					{
						return aula;
					}
				}
			}
		}
		
		return null;
	}
	
	protected AtendimentoRelatorioDTO getProximaAula(int linhaGradeHoraria, int colunaGradeHoraria){
		int diaDaSemanaQueEstahSendoDesenhado = colunaGradeHoraria;
		int linhaDaGradeEmQueAulaDeveSerDesenhada = 1; // a primeira aula do dia deve ser desenhada na linha 1
		if(this.atendimentoDTO != null){
			for(AtendimentoRelatorioDTO aula : this.atendimentoDTO){
				// verifica se a aula em questão corresponde ao dia da semana que será desenhado
				if(aula.getSemana() == diaDaSemanaQueEstahSendoDesenhado){
					// verifica se a linha da grade que está sendo desenhada corresponde à linha da grade em que a aula em questão
					// deve ser desenhada
					if(linhaGradeHoraria == linhaDaGradeEmQueAulaDeveSerDesenhada) return aula;
					
					// calcula a quantidade de linhas, para cada crédito, que a aula em questão ocupa na grade horária
					int qtdLinhasNaGradeHorariaPorCreditoDaAula = aula.getDuracaoDeUmaAulaEmMinutos() / mdcTemposAulaNumSemanasLetivas.getPrimeiro();

					// calcula a linha em que a próxima aula do dia da semana deve ser desenhada
					linhaDaGradeEmQueAulaDeveSerDesenhada += aula.getTotalCreditos() * qtdLinhasNaGradeHorariaPorCreditoDaAula;
				}
			}
		}

		return null;
	}
	
	private Integer calculaTamanhoLabelReduzida(
			String horarioASerEscrito) {
		String[] horarioInicialArray = horarioASerEscrito.substring(0, 5).split(":");
		int horarioInicialHoras = Integer.parseInt(horarioInicialArray[0]);
		int horarioInicialMinutos = Integer.parseInt(horarioInicialArray[1]);
		
		String[] horarioFinalArray = horarioASerEscrito.substring(8, 13).split(":");
		int horarioFinalHoras = Integer.parseInt(horarioFinalArray[0]);
		int horarioFinalMinutos = Integer.parseInt(horarioFinalArray[1]);
		
		int difHoras = horarioFinalHoras - horarioInicialHoras;
		int difMinutos = horarioFinalMinutos - horarioInicialMinutos;
		
		int tamanhoLabel = (difHoras * 60) + (difMinutos);
		if (tamanhoLabel > 60)
			tamanhoLabel = 60;
		if (tamanhoLabel <= 10)
			tamanhoLabel = 15;
		
		return tamanhoLabel;
	}
	
	private Integer calculaPosicaoAula(int rowIndex)
	{
		int posicao = 0;
		for (int i = 0; i < rowIndex; i++)
		{
			if (isHorarioIntervalo(horariosEscritos.get(i)))
			{
				posicao += 12;
			}
			else
			{
			posicao +=  (tamanhoLinhaGradeHorariaEmPixels *
					(calculaTamanhoLabelReduzida(horariosEscritos.get(i))/15));
			}
		}
		
		return posicao;
	}
	
	protected String getHorarioFinalAula(AtendimentoRelatorioDTO aula)
	{
		String pattern = "00";
		String[] horarioInicialArray = aula.getHorarioAulaString().split(":");
		int horarioInicialHoras = Integer.parseInt(horarioInicialArray[0]);
		int horarioInicialMinutos = Integer.parseInt(horarioInicialArray[1]);
		
		
		int horarioFinalHoras = horarioInicialHoras;
		int horarioFinalMinutos = horarioInicialMinutos + (aula.getDuracaoDeUmaAulaEmMinutos() * aula.getTotalCreditos());
		while (horarioFinalMinutos >= 60)
		{
			horarioFinalHoras++;
			horarioFinalMinutos -= 60;
		}
		return (NumberFormat.getFormat(pattern).format(horarioFinalHoras) + ":" + NumberFormat.getFormat(pattern).format(horarioFinalMinutos));
	}
	
	protected int getIndexHorario(String horario, List<String> labels)
	{
		for (String label : labels)
		{
			if (label.substring(0, 5).equals(horario))
			{
				return labels.indexOf(label);
			}
		}
		return labels.size();
	}
	
	public String getCssDisciplina(long id) {
		if (this.disciplinaIdToCSSStyleMap.isEmpty()) {
			int index = this.disciplinasCores.indexOf(id);
	
			if(index < 0 || index > 14) return "corDisciplina14";
	
			return "corDisciplina" + index;
		} else {
			return this.disciplinaIdToCSSStyleMap.get(id);
		}
	}
	
	public void preencheCores(){
		Set<Long> set = new HashSet<Long>();

		for(AtendimentoRelatorioDTO a : this.atendimentoDTO){
			set.add(a.getDisciplinaId());
		}

		this.disciplinasCores.clear();
		this.disciplinasCores.addAll(set);
	}
	
	public void preencheCoresElevaEducacao() {
		this.disciplinaIdToCSSStyleMap.clear();
		
		Map<Long, String> disciplinaIdToSiglaMap = new HashMap<Long, String>();
		Set<String> disciplinasSiglas = new HashSet<String>();
		
		for(AtendimentoRelatorioDTO a : this.atendimentoDTO){
			String disSigla = TriedaUtil.getSiglaDisciplinaParaEscola(a.getDisciplinaOriginalCodigo());
//			String disSigla = a.getDisciplinaOriginalCodigo();
//			if (disSigla.contains("_")) {
//				disSigla = disSigla.split("_")[0];
//			}
			disciplinasSiglas.add(disSigla);
			disciplinaIdToSiglaMap.put(a.getDisciplinaId(), disSigla);
		}
		
		// ordena disciplinas e monta mapa de cores por disciplina
		List<String> disciplinasOrdenadas = new ArrayList<String>(disciplinasSiglas);
		Collections.sort(disciplinasOrdenadas);
		Map<String, String> disciplinaSiglaToCorMap = new HashMap<String, String>();
		for (String disciplinaSigla : disciplinasOrdenadas) {
			String cssStyle = "corDisciplina" + (disciplinaSiglaToCorMap.size() % 14);
			disciplinaSiglaToCorMap.put(disciplinaSigla,cssStyle);
		}
		for (Entry<Long, String> entry : disciplinaIdToSiglaMap.entrySet()) {
			Long disId = entry.getKey();
			String disSigla = disciplinaIdToSiglaMap.get(disId);
			String cssStyle = disciplinaSiglaToCorMap.get(disSigla);
			this.disciplinaIdToCSSStyleMap.put(disId,cssStyle);
		}
	}
	
	public class LinhaDeCredito extends BaseModel{
		private static final long serialVersionUID = 3996652461744817138L;
		private int linhaNaGradeHoraria;

		public LinhaDeCredito(){}
		
		public LinhaDeCredito(String display, int linhaNaGradeHoraria){
			this.setDisplay(display);
			this.linhaNaGradeHoraria = linhaNaGradeHoraria;
		}

		public String getDisplay(){
			return get("display");
		}
		
		public int getLinhaNaGradeHoraria(){
			return linhaNaGradeHoraria;
		}

		public void setDisplay(String value){
			set("display", value);
		}

		public String getSegunda(){
			return get("segunda");
		}

		public void setSegunda(String value){
			set("segunda", value);
		}

		public String getTerca(){
			return get("terca");
		}

		public void setTerca(String value){
			set("terca", value);
		}

		public String getQuarta(){
			return get("quarta");
		}

		public void setQuarta(String value){
			set("quarta", value);
		}

		public String getQuinta(){
			return get("quinta");
		}

		public void setQuinta(String value){
			set("quinta", value);
		}

		public String getSexta(){
			return get("sexta");
		}

		public void setSexta(String value){
			set("sexta", value);
		}

		public String getSabado(){
			return get("sabado");
		}

		public void setSabado(String value){
			set("sabado", value);
		}

		public String getDomingo(){
			return get("domingo");
		}

		public void setDomingo(String value){
			set("domingo", value);
		}
		
	}
	
	public void setEmptyText(String text){
		this.grid.getView().setEmptyText(text);
	}
	
	public TurnoDTO getTurnoDTO(){
		return this.turnoDTO;
	}

	public void setTurnoDTO(TurnoDTO turnoDTO){
		this.turnoDTO = turnoDTO;
	}
	
	public void setEmptyTextBeforeSearch(String text){
		this.emptyTextBeforeSearch = text;
	}
	
	public void setAulaDestaque(AulaDTO aulaDestaque){
		this.aulaDestaque = aulaDestaque;
	}
	
	public Grid<LinhaDeCredito> getGrid()
	{
		return this.grid;
	}
	
}
