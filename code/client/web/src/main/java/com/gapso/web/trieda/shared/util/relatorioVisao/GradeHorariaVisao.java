package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.core.El;
import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.dnd.DragSource;
import com.extjs.gxt.ui.client.dnd.GridDropTarget;
import com.extjs.gxt.ui.client.event.DNDEvent;
import com.extjs.gxt.ui.client.event.DNDListener;
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
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.dom.client.Style.Unit;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public abstract class GradeHorariaVisao extends ContentPanel{
	protected Grid<LinhaDeCredito> grid;
	protected ListStore<LinhaDeCredito> store;
	protected boolean temInfoDeHorarios;
	protected ParDTO<Integer, Integer> mdcTemposAulaNumSemanasLetivas;
	protected Integer tamanhoLinhaGradeHorariaEmPixels;
	protected List<AtendimentoRelatorioDTO> atendimentoDTO;
	protected List<String> labelsDasLinhasDaGradeHoraria;
	protected List<String> horariosDeInicioDeAula;
	protected List<Boolean> horarioEhIntervalo;
	protected TurnoDTO turnoDTO;
	protected QuickTip quickTip;
	protected List<Long> disciplinasCores = new ArrayList<Long>();

	protected String emptyTextBeforeSearch = "Preencha o filtro acima";
	protected String emptyTextAfterSearch = "Não foi encontrado nenhuma Grade Horária para este filtro";
	
	public GradeHorariaVisao(){
		super(new FitLayout());
		this.temInfoDeHorarios = true;
		this.mdcTemposAulaNumSemanasLetivas = ParDTO.create(1, 1);
		this.tamanhoLinhaGradeHorariaEmPixels = 0;
		this.labelsDasLinhasDaGradeHoraria = new ArrayList<String>();
		this.horariosDeInicioDeAula = new ArrayList<String>();
		this.horarioEhIntervalo = new ArrayList<Boolean>();
		this.setHeaderVisible(false);
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
	}
	
	public abstract RelatorioVisaoFiltro getFiltro();
	public abstract void setFiltro(RelatorioVisaoFiltro filtro);
	public abstract void requestAtendimentos();
	
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
				labelsDasLinhasDaGradeHoraria.clear();
				horariosDeInicioDeAula.clear();
				horarioEhIntervalo.clear();
				atendimentoDTO = result.getAtendimentosDTO();
				if(!atendimentoDTO.isEmpty()){
					temInfoDeHorarios = (atendimentoDTO.iterator().next().getHorarioAulaId() != null);
					mdcTemposAulaNumSemanasLetivas = result.getMdcTemposAulaNumSemanasLetivas();
					if (temInfoDeHorarios) {
						TrioDTO<List<String>,List<String>, List<Boolean>> trioDTO = GradeHoraria.processaLabelsDasLinhasDaGradeHoraria(result.getLabelsDasLinhasDaGradeHoraria(),result.getHorariosDeInicioDeAula(),result.getHorariosDeFimDeAula());
						labelsDasLinhasDaGradeHoraria.addAll(trioDTO.getPrimeiro());
						horariosDeInicioDeAula.addAll(trioDTO.getSegundo());//horariosDeInicioDeAula.addAll(result.getHorariosDeInicioDeAula());
						horarioEhIntervalo.addAll(trioDTO.getTerceiro());					
					} else {
						labelsDasLinhasDaGradeHoraria.addAll(result.getLabelsDasLinhasDaGradeHoraria());
						for (int i = 0; i < result.getLabelsDasLinhasDaGradeHoraria().size(); i++) {
							horarioEhIntervalo.add(false);
						}
					}
					tamanhoLinhaGradeHorariaEmPixels = (int)(GradeHoraria.PIXELS_POR_MINUTO * mdcTemposAulaNumSemanasLetivas.getPrimeiro());
					List<ColumnConfig> columns = getColumnList(result.getQtdColunasPorDiaSemana());
					preencheCores();
					grid.reconfigure(getListStore(), new ColumnModel(columns));
					grid.getView().setEmptyText(emptyTextAfterSearch);
					int totalLinhas = labelsDasLinhasDaGradeHoraria.size();
					for(int row = 0; row < totalLinhas; row++){
						if(horarioEhIntervalo.get(row) && mdcTemposAulaNumSemanasLetivas.getSegundo() < 2) {
							grid.getView().getRow(row).getStyle().setHeight(10, Unit.PX);
						}
						else {
							grid.getView().getRow(row).getStyle().setHeight(tamanhoLinhaGradeHorariaEmPixels - 2, Unit.PX);
						}
					}
				}
				else{
					mdcTemposAulaNumSemanasLetivas = ParDTO.create(1, 1);
					tamanhoLinhaGradeHorariaEmPixels = 0;
					grid.reconfigure(getListStore(), new ColumnModel(getColumnList(null)));
				}
				grid.unmask();
			}
		};
	}
	
	public ListStore<LinhaDeCredito> getListStore(){
		if(this.store == null) this.store = new ListStore<LinhaDeCredito>();
		else this.store.removeAll();

		for (int i = 0; i < labelsDasLinhasDaGradeHoraria.size(); i++) {
			if(horarioEhIntervalo.get(i) && mdcTemposAulaNumSemanasLetivas.getSegundo() < 2) {
				this.store.add(new LinhaDeCredito("", i));
			}
			else {
				this.store.add(new LinhaDeCredito(labelsDasLinhasDaGradeHoraria.get(i), i));
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
					html.setStyleAttribute("line-height", tamanhoLinhaGradeHorariaEmPixels - 2 + "px");
					html.setStyleAttribute("margin-top", "-4px");
					return html;
				}

				return content(model,rowIndex,colIndex);
			}

			private Html content(LinhaDeCredito model, int rowIndex, int colIndex){
				if(atendimentoDTO == null || atendimentoDTO.isEmpty()) return new Html("");

				int semana = getSemana(colIndex);

				AtendimentoRelatorioDTO aulaDTO = null;
				if (temInfoDeHorarios) {
					aulaDTO = getAulaPorHorario(model.getLinhaNaGradeHoraria(),semana);
				} else {
					aulaDTO = getProximaAula(rowIndex+1,semana);
				}

				if (aulaDTO == null) {
					return new Html("");
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
				html.setStyleAttribute("top", ((rowIndex-getNumeroIntervalos(rowIndex)) * tamanhoLinhaGradeHorariaEmPixels + 1 + (10 * getNumeroIntervalos(rowIndex))) + "px");
				// calcula a quantidade de linhas, para cada crédito, que a aula em questão ocupa na grade horária
				int qtdLinhasNaGradeHorariaPorCreditoDaAula = aulaDTO.getDuracaoDeUmaAulaEmMinutos() / mdcTemposAulaNumSemanasLetivas.getPrimeiro();
				html.setStyleAttribute("height", (aulaDTO.getTotalCreditos() * qtdLinhasNaGradeHorariaPorCreditoDaAula * tamanhoLinhaGradeHorariaEmPixels - 3) + "px");
				html.addStyleName("s" + aulaDTO.getSemana()); // Posiciona na coluna ( dia semana )
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
	
	protected int getNumeroIntervalos(int rowIndex) {
		int numeroIntervalos = 0;
		if (mdcTemposAulaNumSemanasLetivas.getSegundo() > 1)
			return 0;	
		for(int i = 0; i<rowIndex; i++){
			if(horarioEhIntervalo.get(i)){
				numeroIntervalos++;
			}
		}
		return numeroIntervalos;
	}

	protected int getSemana(int colIndex){
		int semana = -1;
		if(colIndex > 0 || colIndex < 7) semana = colIndex++;
		
		return semana;
	}
	
	protected abstract TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO);
	
	protected AtendimentoRelatorioDTO getAulaPorHorario(int linhaGradeHoraria, int colunaGradeHoraria){
		if(this.atendimentoDTO != null){
			String labelHorario = horariosDeInicioDeAula.get(linhaGradeHoraria);
			for(AtendimentoRelatorioDTO aula : this.atendimentoDTO){
				if(aula.getSemana() == colunaGradeHoraria){
					if(labelHorario.contains(aula.getHorarioAulaString())) return aula;
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
	
	public String getCssDisciplina(long id){
		int index = this.disciplinasCores.indexOf(id);

		if(index < 0 || index > 14) return "corDisciplina14";

		return "corDisciplina" + index;
	}
	
	public void preencheCores(){
		Set<Long> set = new HashSet<Long>();

		for(AtendimentoRelatorioDTO a : this.atendimentoDTO){
			set.add(a.getDisciplinaId());
		}

		this.disciplinasCores.clear();
		this.disciplinasCores.addAll(set);
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
	
	public TurnoDTO getTurnoDTO(){
		return this.turnoDTO;
	}

	public void setTurnoDTO(TurnoDTO turnoDTO){
		this.turnoDTO = turnoDTO;
	}
	
}
