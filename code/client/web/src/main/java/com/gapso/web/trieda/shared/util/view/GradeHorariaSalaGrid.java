package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
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
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.QuartetoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.dom.client.Style.Unit;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaSalaGrid
	extends ContentPanel
{
	private Grid< LinhaDeCredito > grid;
	private ListStore< LinhaDeCredito > store;
	private Integer mdcTemposAula;
	private Integer maximoCreditos;
	private Integer qtdLinhasGradeHorariaPorCredito;
	private Integer tamanhoLinhaGradeHorariaEmPixels;
	private List<AtendimentoRelatorioDTO> aulasNaSalaDTO;
	private SalaDTO salaDTO;
	private TurnoDTO turnoDTO;
	private QuickTip quickTip;
	private List< Long > disciplinasCores = new ArrayList< Long >();

	private String emptyTextBeforeSearch = "Preencha o filtro acima";
	private String emptyTextAfterSearch = "Não foi encontrado nenhuma Grade Horária para este filtro";

	public GradeHorariaSalaGrid()
	{
		super( new FitLayout() );
		this.setHeaderVisible( false );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();

		this.grid = new Grid<LinhaDeCredito>(getListStore(),new ColumnModel(getColumnList()));

		this.grid.setTrackMouseOver( false );
		this.grid.setStyleName( "GradeHorariaGrid VisaoSala" );
		
		this.grid.addListener( Events.BeforeSelect,
			new Listener< GridEvent< LinhaDeCredito > >()
		{
			@Override
			public void handleEvent(
				GridEvent< LinhaDeCredito > be )
			{
				be.setCancelled( true );
			}
		});

		this.grid.getView().setEmptyText( this.emptyTextBeforeSearch );
		this.quickTip = new QuickTip( this.grid );
		this.quickTip.getToolTipConfig().setDismissDelay( 0 );
		add( this.grid );

		GridDropTarget target = new GridDropTarget( this.grid )
		{
			@Override
			protected void onDragDrop( DNDEvent event ) { }
		};

		target.addDNDListener( new DNDListener()
		{
			@Override
			public void dragMove( DNDEvent e )
			{
				int linha = grid.getView().findRowIndex(
					e.getDragEvent().getTarget() );

				int coluna = grid.getView().findCellIndex(
					e.getDragEvent().getTarget(), null );

				if ( linha < 0 || coluna < 1 )
				{
					e.setCancelled( true );
					e.getStatus().setStatus( false );
					return;
				}

				int semana = coluna + 1;
				semana = ( ( semana == 8 ) ? 1 : semana );
				e.setCancelled( false );
				e.getStatus().setStatus( true );

				return;
			}
		});

		requestAtendimentos();
	}

	public void requestAtendimentos() {
		if (getSalaDTO() == null || getTurnoDTO() == null) {
			return;
		}

		this.grid.mask("Carregando os dados, " + "aguarde alguns instantes", "loading");

		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoSala(getSalaDTO(),getTurnoDTO(),/*getSemanaLetivaDTO(),*/new AsyncCallback<QuartetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>>>() {
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!","Não foi possível carregar a grade de horários",null);
			}

			@Override
			public void onSuccess(QuartetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>> result) {
				mdcTemposAula = result.getPrimeiro();
				maximoCreditos = result.getSegundo();
				Integer tempoAulaDaSemanaLetivaComMaximoDeCreditos = result.getTerceiro();
				aulasNaSalaDTO = result.getQuarto();
				qtdLinhasGradeHorariaPorCredito = tempoAulaDaSemanaLetivaComMaximoDeCreditos/mdcTemposAula;
				tamanhoLinhaGradeHorariaEmPixels = (int)(GradeHoraria.PIXELS_POR_MINUTO*mdcTemposAula);
				
				preencheCores();
				grid.reconfigure(getListStore(),new ColumnModel(getColumnList()));
				grid.getView().setEmptyText(emptyTextAfterSearch);
				for (int row = 0; row < maximoCreditos*qtdLinhasGradeHorariaPorCredito; row++) {
					grid.getView().getRow(row).getStyle().setHeight(tamanhoLinhaGradeHorariaEmPixels-2,Unit.PX);
				}
				grid.unmask();
			}
		});
	}

	public ListStore<LinhaDeCredito> getListStore() {
		if (this.store == null) {
			this.store = new ListStore<LinhaDeCredito>();
		}
		else {
			this.store.removeAll();
		}

		Set<LinhaDeCredito> setLinhaDeCredito = new HashSet<LinhaDeCredito>();
		
		if (isTatico()) {
			Integer cargaHorariaAcumuladaEmMinutos = mdcTemposAula;
			for (int indexCredito = 1; indexCredito <= maximoCreditos; indexCredito++) {
				for (int i = 0; i < qtdLinhasGradeHorariaPorCredito; i++) {
					setLinhaDeCredito.add(new LinhaDeCredito(cargaHorariaAcumuladaEmMinutos.toString()));
					cargaHorariaAcumuladaEmMinutos += mdcTemposAula;
				}
			}
		}
		else
		{
			// FIXME implementar caso para o operacional
		}

//		if (this.semanaLetivaDTO != null) {
//			if ( isTatico() )
//			{
//				for ( Integer i = 1; i <= this.semanaLetivaDTO.getMaxCreditos(); i++ )
//				{
//					setLinhaDeCredito.add( new LinhaDeCredito( i.toString() ) );
//				}
//			}
//			else
//			{
//				for ( Long horarioId : this.semanaLetivaDTO.getHorariosStringMap().keySet() )
//				{
//					setLinhaDeCredito.add( new LinhaDeCredito(
//						this.semanaLetivaDTO.getHorariosStringMap().get( horarioId ),
//						horarioId, this.semanaLetivaDTO.getHorariosInicioMap().get( horarioId ) ) );
//				}
//			}
//		}

		List<LinhaDeCredito> listLinhaDeCredito = new ArrayList<LinhaDeCredito>();

		listLinhaDeCredito.addAll(setLinhaDeCredito);
		Collections.sort(listLinhaDeCredito);

		for (LinhaDeCredito lc : listLinhaDeCredito) {
			this.store.add(lc);
		}

		return this.store;
	}

	@Override
	protected void onRender( Element parent, int pos )
	{
		super.onRender( parent, pos );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		if ( isTatico() )
		{
			addColumn( list, "display", "Carga Horária (Min)" );
		}
		else
		{
			addColumn( list, "display", "Horários" );
		}

		addColumn( list, "segunda", "Segunda" );
		addColumn( list, "terca", "Terça" );
		addColumn( list, "quarta", "Quarta" );
		addColumn( list, "quinta", "Quinta" );
		addColumn( list, "sexta", "Sexta" );
		addColumn( list, "sabado", "Sábado" );
		addColumn( list, "domingo", "Domingo" );

		return list;
	}

	private void addColumn( List< ColumnConfig > list, String id, String name )
	{
		GridCellRenderer< LinhaDeCredito > change = new GridCellRenderer<LinhaDeCredito>() {
			public Html render(LinhaDeCredito model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<LinhaDeCredito> store, Grid<LinhaDeCredito> grid) {
				if (colIndex == 0) {
					Html html = new Html(model.getDisplay());
					html.setStyleAttribute("line-height",tamanhoLinhaGradeHorariaEmPixels-2+"px");
					return html;
				}

				return content(model,rowIndex,colIndex);
			}

			private Html content(LinhaDeCredito model, int rowIndex, int colIndex) {
				if (aulasNaSalaDTO == null || aulasNaSalaDTO.isEmpty()) {
					return new Html("");
				}

				int semana = -1;
				if (colIndex == 1) {
					semana = 2;
				}
				else if (colIndex == 2) {
					semana = 3;
				}
				else if (colIndex == 3) {
					semana = 4;
				}
				else if (colIndex == 4) {
					semana = 5;
				}
				else if (colIndex == 5) {
					semana = 6;
				}
				else if (colIndex == 6) {
					semana = 7;
				}
				else if (colIndex == 7) {
					semana = 1;
				}

				AtendimentoRelatorioDTO aulaDTO = null;
				if (isTatico()) {
					aulaDTO = getAula(rowIndex+1,semana);
				} else {
					aulaDTO = getAtendimento(model.getHorarioId(),semana);
				}

				if (aulaDTO == null) {
					return new Html("");
				}

				final String title = aulaDTO.getDisciplinaString();
				final String contentToolTip = aulaDTO.getContentToolTipVisaoSala(ReportType.WEB);
				final Html html = new Html(aulaDTO.getContentVisaoSala(ReportType.WEB)) {
					@Override
					protected void onRender(Element target, int index) {
						super.onRender(target,index);
						target.setAttribute("qtip",contentToolTip);
						target.setAttribute("qtitle",title);
						target.setAttribute("qwidth","400px");
					}
				};
				html.addStyleName("horario");
				if (isTatico()) {
					html.setStyleAttribute("top",(rowIndex*tamanhoLinhaGradeHorariaEmPixels + 1)+"px");//html.addStyleName("cargaHoraria" + (rowIndex+1));
					// calcula a quantidade de linhas, para cada crédito, que a aula em questão ocupa na grade horária
					int qtdLinhasNaGradeHorariaPorCreditoDaAula = aulaDTO.getSemanaLetivaTempoAula()/mdcTemposAula;
					html.setStyleAttribute("height",(aulaDTO.getTotalCreditos()*qtdLinhasNaGradeHorariaPorCreditoDaAula*tamanhoLinhaGradeHorariaEmPixels - 3)+"px");//html.addStyleName("totalCargaHoraria" + aulaDTO.getTotalCreditos()*qtdLinhasNaGradeHorariaPorCreditoDaAula);
				} else {
					html.addStyleName("c" + (rowIndex+1));
					html.addStyleName("tc" + aulaDTO.getTotalCreditos());
				}
				html.addStyleName("s" + aulaDTO.getSemana());
				html.addStyleName(getCssDisciplina(aulaDTO.getDisciplinaId()));

				new DragSource( html )
				{
					@Override
					protected void onDragStart( DNDEvent event )
					{
						event.setData( html );
						event.getStatus().update(
							El.fly( html.getElement() ).cloneNode( true ) );

						quickTip.hide();
					}
				};

				return html;
			}
		};

		ColumnConfig column = new ColumnConfig( id, name, 100 );
		column.setRenderer( change );
		column.setResizable( false );
		column.setMenuDisabled( true );
		column.setSortable( false );

		list.add( column );
	}

	private AtendimentoRelatorioDTO getAtendimento( Long horarioId, int semana )
	{
		int ocupado = 0;

		if ( this.aulasNaSalaDTO != null )
		{
			for ( AtendimentoRelatorioDTO at : this.aulasNaSalaDTO )
			{
				if ( at.getSemana() == semana )
				{
					if ( ( (AtendimentoOperacionalDTO) at).getHorarioId().equals( horarioId ) )
					{
						return at;
					}

					ocupado += at.getTotalCreditos();
				}
			}
		}

		return null;
	}

	private AtendimentoRelatorioDTO getAula(int linhaGradeHoraria, int colunaGradeHoraria) {
		int diaDaSemanaQueEstahSendoDesenhado = colunaGradeHoraria;
		int linhaDaGradeEmQueAulaDeveSerDesenhada = 1; // a primeira aula do dia deve ser desenhada na linha 1
		if (this.aulasNaSalaDTO != null) {
			for (AtendimentoRelatorioDTO aula : this.aulasNaSalaDTO) {
				// verifica se a aula em questão corresponde ao dia da semana que será desenhado
				if (aula.getSemana() == diaDaSemanaQueEstahSendoDesenhado) {
					// verifica se a linha da grade que está sendo desenhada corresponde à linha da grade em que a aula em questão
					// deve ser desenhada
					if (linhaGradeHoraria == linhaDaGradeEmQueAulaDeveSerDesenhada) {
						return aula;
					}
					
					// calcula a quantidade de linhas, para cada crédito, que a aula em questão ocupa na grade horária
					int qtdLinhasNaGradeHorariaPorCreditoDaAula = aula.getSemanaLetivaTempoAula()/mdcTemposAula;

					// calcula a linha em que a próxima aula do dia da semana deve ser desenhada
					linhaDaGradeEmQueAulaDeveSerDesenhada += aula.getTotalCreditos()*qtdLinhasNaGradeHorariaPorCreditoDaAula;
				}
			}
		}

		return null;
	}

	private boolean isTatico()
	{
		if ( this.aulasNaSalaDTO == null )
		{
			return false;
		}

		if ( this.aulasNaSalaDTO.isEmpty() )
		{
			return true;
		}

		AtendimentoRelatorioDTO atm
			= this.aulasNaSalaDTO.get( 0 );

		if ( atm == null )
		{
			return false;
		}

		return ( atm instanceof AtendimentoTaticoDTO );
	}

	public SalaDTO getSalaDTO()
	{
		return this.salaDTO;
	}

	public void setSalaDTO( SalaDTO salaDTO )
	{
		this.salaDTO = salaDTO;
	}

	public TurnoDTO getTurnoDTO()
	{
		return this.turnoDTO;
	}

	public void setTurnoDTO( TurnoDTO turnoDTO )
	{
		this.turnoDTO = turnoDTO;
	}

	public String getCssDisciplina( long id )
	{
		int index = this.disciplinasCores.indexOf( id );

		if ( index < 0 || index > 14 )
		{
			return "corDisciplina14";
		}

		return "corDisciplina" + index;
	}

	public void preencheCores()
	{
		Set< Long > set = new HashSet< Long >();

		for ( AtendimentoRelatorioDTO a : this.aulasNaSalaDTO )
		{
			set.add( a.getDisciplinaId() );
		}

		this.disciplinasCores.clear();
		this.disciplinasCores.addAll( set );
	}

	public class LinhaDeCredito
		extends BaseModel
		implements Comparable< LinhaDeCredito >
	{
		private static final long serialVersionUID = 3996652461744817138L;

		private Date horarioInicio;

		public Date getHorarioInicio()
		{
			return this.horarioInicio;
		}

		public void setHorarioInicio( Date horarioInicio )
		{
			this.horarioInicio = horarioInicio;
		}

		public LinhaDeCredito( String display )
		{
			this( display, null, null );
		}

		public LinhaDeCredito(
			String display, Long horarioId, Date horarioInicio )
		{
			this.setDisplay( display );
			this.setHorarioId( horarioId );
			this.horarioInicio = horarioInicio;
		}

		public String getDisplay()
		{
			return get( "display" );
		}

		public void setDisplay( String value )
		{
			set( "display", value );
		}

		public Long getHorarioId()
		{
			return get( "horarioId" );
		}

		public void setHorarioId( Long value )
		{
			set( "horarioId", value );
		}

		public Integer getTotalCreditos()
		{
			return get( "totalCreditos" );
		}

		public void setTotalCreditos( Integer value )
		{
			set( "totalCreditos", value );
		}

		public String getSegunda()
		{
			return get( "segunda" );
		}

		public void setSegunda( String value )
		{
			set( "segunda", value );
		}

		public String getTerca()
		{
			return get( "terca" );
		}

		public void setTerca( String value )
		{
			set( "terca", value );
		}

		public String getQuarta()
		{
			return get( "quarta" );
		}

		public void setQuarta( String value )
		{
			set( "quarta", value );
		}

		public String getQuinta()
		{
			return get( "quinta" );
		}

		public void setQuinta( String value )
		{
			set( "quinta", value );
		}

		public String getSexta()
		{
			return get( "sexta" );
		}

		public void setSexta( String value )
		{
			set( "sexta", value );
		}

		public String getSabado()
		{
			return get( "sabado" );
		}

		public void setSabado( String value )
		{
			set( "sabado", value );
		}

		public String getDomingo()
		{
			return get( "domingo" );
		}

		public void setDomingo( String value )
		{
			set( "domingo", value );
		}

		@Override
		public int compareTo( LinhaDeCredito o )
		{
			if ( o == null )
			{
				return 1;
			}

			if ( this.getHorarioInicio() == null
				&& o.getHorarioInicio() == null )
			{
				// Se os horários de início são nulos, estamos no modelo tático,
				// onde o valor de 'display' corresponde ao número de créditos do turno
				// Ex.: 1, 2, 3, 4...
				String str1 = ( this.getDisplay() == null ? "" : this.getDisplay() );
				String str2 = ( o.getDisplay() == null ? "" : o.getDisplay() );

				Integer i = Integer.parseInt( str1 );
				Integer j = Integer.parseInt( str2 );

				return i.compareTo( j );
			}

			if ( this.getHorarioInicio() != null
				&& o.getHorarioInicio() == null )
			{
				return 1;
			}

			if ( this.getHorarioInicio() == null
				&& o.getHorarioInicio() != null )
			{
				return -1;
			}

			return this.getHorarioInicio().compareTo( o.getHorarioInicio() );
		}
	}
}
