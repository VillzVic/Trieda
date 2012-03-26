package com.gapso.web.trieda.shared.util.view;

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
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.dom.client.Style.Unit;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaCursoGrid
	extends ContentPanel
{
	private Grid< LinhaDeCredito > grid;
	private ListStore< LinhaDeCredito > store;
	private Integer mdcTemposAula;
	private Integer tamanhoLinhaGradeHorariaEmPixels;
	private List< AtendimentoRelatorioDTO > aulasDoBlocoCurricularDTO;
	private List< Integer > qtdColunasPorDiaSemana;
	private List<String> labelsDasLinhasDaGradeHoraria;
	private CurriculoDTO curriculoDTO;
	private int periodo;
	private TurnoDTO turnoDTO;
	private CampusDTO campusDTO;
	private CursoDTO cursoDTO;
	private QuickTip quickTip;
	private List< Long > disciplinasCores = new ArrayList< Long >();

	private String emptyTextBeforeSearch = "Preencha o filtro acima";
	private String emptyTextAfterSearch = "Não foi encontrado nenhuma Grade Horária para este filtro";

	public GradeHorariaCursoGrid() {
		super( new FitLayout() );
		this.mdcTemposAula = 1;
		this.tamanhoLinhaGradeHorariaEmPixels = 0;
		this.labelsDasLinhasDaGradeHoraria = new ArrayList<String>();
		setHeaderVisible( false );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();

		this.grid = new Grid< LinhaDeCredito >( getListStore(),
			new ColumnModel( getColumnList() ) );

		this.grid.setTrackMouseOver( false );
		this.grid.setStyleName( "GradeHorariaGrid VisaoSala" ); // TODO lembrar de eliminar o CSS de visao sala
		this.grid.addListener( Events.BeforeSelect,
			new Listener< GridEvent< LinhaDeCredito > >()
			{
				@Override
				public void handleEvent( GridEvent< LinhaDeCredito > be )
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

				e.setCancelled( false );
				e.getStatus().setStatus( true );

				return;
			}
		});

		requestAtendimentos();
	}

	public void requestAtendimentos()
	{
		if ( getCurriculoDTO() == null || getTurnoDTO() == null
			|| getCampusDTO() == null || getPeriodo() <= 0 )
		{
			return;
		}

		this.grid.mask( "Carregando os dados, "
			+ "aguarde alguns instantes", "loading" );

//		SemanasLetivaServiceAsync semanasLetivaService = Services.semanasLetiva();
//		semanasLetivaService.findSemanaLetiva( getCurriculoDTO().getSemanaLetivaId(),
//			new AsyncCallback< SemanaLetivaDTO >()
//		{
//			@Override
//			public void onFailure( Throwable caught )
//			{
//				semanaLetivaDTO = null;
//			}
//
//			@Override
//			public void onSuccess( SemanaLetivaDTO result )
//			{
//				Map<Long, Date> horariosPermitidos = getTurnoDTO().getHorariosInicioMap();
//				Map<Long, Date> novoInicioMap = new HashMap<Long, Date>();
//				Map<Long, String> novoStringMap = new HashMap<Long, String>();
//				
//				// Remove os horarios que nao sao compativeis com o turno escolhido.
//				// Devido as injecoes do Spring, nao pode-se remover os horarios do
//				// objeto result. Por isso a necessidade de criar-se Maps auxiliares
//				// para depois seta-lo como propriedade do result.
//				for(Long key: result.getHorariosInicioMap().keySet()){
//					if(horariosPermitidos.containsKey(key)){
//						novoInicioMap.put(key, result.getHorariosInicioMap().get(key));
//						novoStringMap.put(key, result.getHorariosStringMap().get(key));
//					}
//				}
//				result.setHorariosInicioMap(novoInicioMap);
//				result.setHorariosStringMap(novoStringMap);
//				semanaLetivaDTO = result;
//			}
//		});

		AtendimentosServiceAsync serviceAtendimentos = Services.atendimentos();
		serviceAtendimentos.getAtendimentosParaGradeHorariaVisaoCurso(getCurriculoDTO(),getPeriodo(),getTurnoDTO(),getCampusDTO(),getCursoDTO(),new AsyncCallback<SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>>() {
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!","Não foi possível carregar a grade de horários",null);
			}

			@Override
			public void onSuccess(SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>> result) {
				labelsDasLinhasDaGradeHoraria.clear();
				aulasDoBlocoCurricularDTO = result.getQuarto();
				if (!aulasDoBlocoCurricularDTO.isEmpty()) {
					mdcTemposAula = result.getPrimeiro();
					tamanhoLinhaGradeHorariaEmPixels = (int)(GradeHoraria.PIXELS_POR_MINUTO*mdcTemposAula);
					qtdColunasPorDiaSemana = result.getQuinto();
					labelsDasLinhasDaGradeHoraria.addAll(result.getSexto());
					preencheCores();
					grid.reconfigure(getListStore(),new ColumnModel(getColumnList()));
					grid.getView().setEmptyText(emptyTextAfterSearch);
					int totalLinhas = isTatico() ? labelsDasLinhasDaGradeHoraria.size() : labelsDasLinhasDaGradeHoraria.size()-1;
					for (int row = 0; row < totalLinhas; row++) {
						grid.getView().getRow(row).getStyle().setHeight(tamanhoLinhaGradeHorariaEmPixels-2,Unit.PX);
					}
				} else {
					mdcTemposAula = 1;
					tamanhoLinhaGradeHorariaEmPixels = 0;
					grid.reconfigure(getListStore(),new ColumnModel(getColumnList()));
				}
				grid.unmask();
			}
		});
	}

	public ListStore<LinhaDeCredito> getListStore() {
		if (this.store == null) {
			this.store = new ListStore<LinhaDeCredito>();
		} else {
			this.store.removeAll();
		}

		if (isTatico()) {
			for (int i = 0; i < labelsDasLinhasDaGradeHoraria.size(); i++) {
				this.store.add(new LinhaDeCredito(labelsDasLinhasDaGradeHoraria.get(i),i));
			}
		} else {
			for (int i = 0; i < labelsDasLinhasDaGradeHoraria.size()-1; i++) {
				String label = labelsDasLinhasDaGradeHoraria.get(i) + " / " + labelsDasLinhasDaGradeHoraria.get(i+1); 
				this.store.add(new LinhaDeCredito(label,i));
			}
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

		int columnsCount = getColumnsCount();

		for ( int i = 8; i < columnsCount; i++ )
		{
			addColumn( list, "extra" + i, "" );
		}

		return list;
	}

	private int getColumnsCount()
	{
		int count = 0;

		if ( this.qtdColunasPorDiaSemana == null )
		{
			return count;
		}

		for ( Integer c : this.qtdColunasPorDiaSemana )
		{
			count += c;
		}

		return count;
	}

	private void addColumn(List<ColumnConfig> list, String id, String name)
	{
		GridCellRenderer<LinhaDeCredito> change = new GridCellRenderer<LinhaDeCredito>() {
			public Html render( LinhaDeCredito model, String property, ColumnData config, int rowIndex, int colIndex, ListStore< LinhaDeCredito > store, Grid< LinhaDeCredito > grid ) {
				if (colIndex == 0) {
					Html html = new Html(model.getDisplay());
					html.setStyleAttribute("line-height",tamanhoLinhaGradeHorariaEmPixels-2+"px");
					return html;
				}

				return content(model,rowIndex,colIndex);
			}

			private Html content(LinhaDeCredito model, int rowIndex, int colIndex) {
				if (aulasDoBlocoCurricularDTO == null || aulasDoBlocoCurricularDTO.isEmpty()) {
					return new Html("");
				}

				int semana = colIndex;
				AtendimentoRelatorioDTO aulaDTO = null;
				if (isTatico()) {
					aulaDTO = getAulaTatico(rowIndex+1,semana);
				} else {
					aulaDTO = getAulaOperacional(model.getLinhaNaGradeHoraria(),semana);
				}

				if (aulaDTO == null) {
					return new Html("");
				}

				final String title = aulaDTO.getDisciplinaString();
				final String contentToolTip = aulaDTO.getContentToolTipVisaoCurso(ReportType.WEB);				
				final Html html = new Html(aulaDTO.getContentVisaoCurso(ReportType.WEB)) {
					@Override
					protected void onRender(Element target, int index) {
						super.onRender(target,index);
						target.setAttribute("qtip",contentToolTip);
						target.setAttribute("qtitle",title);
						target.setAttribute("qwidth","400px");
					}
				};
				html.addStyleName("horario");
				html.setStyleAttribute("top",(rowIndex*tamanhoLinhaGradeHorariaEmPixels + 1)+"px");
				// calcula a quantidade de linhas, para cada crédito, que a aula em questão ocupa na grade horária
				int qtdLinhasNaGradeHorariaPorCreditoDaAula = aulaDTO.getDuracaoDeUmaAulaEmMinutos()/mdcTemposAula;
				html.setStyleAttribute("height",(aulaDTO.getTotalCreditos()*qtdLinhasNaGradeHorariaPorCreditoDaAula*tamanhoLinhaGradeHorariaEmPixels - 3)+"px");
				html.addStyleName("s" + aulaDTO.getSemana()); // Posiciona na coluna ( dia semana )
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

		int width = getWidth( id );

		if ( name.equals( "" ) )
		{
			width = 0;
		}

		ColumnConfig column = new ColumnConfig( id, name, width );
		column.setRenderer( change );
		column.setResizable( false );
		column.setMenuDisabled( true );
		column.setSortable( false );
		list.add( column );
	}

	private AtendimentoRelatorioDTO getAulaOperacional(int linhaGradeHoraria, int colunaGradeHoraria) {
		if (this.aulasDoBlocoCurricularDTO != null) {
			String labelHorario = labelsDasLinhasDaGradeHoraria.get(linhaGradeHoraria);
			for (AtendimentoRelatorioDTO aula : this.aulasDoBlocoCurricularDTO) {
				if (aula.getSemana() == colunaGradeHoraria) {
					AtendimentoOperacionalDTO aulaOperacional = (AtendimentoOperacionalDTO)aula;
					if (aulaOperacional.getHorarioString().equals(labelHorario)) {
						return aula;
					}
				}
			}
		}
		
		return null;
	}

	private AtendimentoRelatorioDTO getAulaTatico(int linhaGradeHoraria, int colunaGradeHoraria) {
		int diaDaSemanaQueEstahSendoDesenhado = colunaGradeHoraria;
		int linhaDaGradeEmQueAulaDeveSerDesenhada = 1;
		if (this.aulasDoBlocoCurricularDTO != null) {
			for (AtendimentoRelatorioDTO aula : this.aulasDoBlocoCurricularDTO) {
				// verifica se a aula em questão corresponde ao dia da semana que será desenhado
				if (aula.getSemana() == diaDaSemanaQueEstahSendoDesenhado) {
					// verifica se a linha da grade que está sendo desenhada corresponde à linha da grade em que a aula em questão
					// deve ser desenhada
					if (linhaGradeHoraria == linhaDaGradeEmQueAulaDeveSerDesenhada) {
						return aula;
					}
					
					// calcula a quantidade de linhas, para cada crédito, que a aula em questão ocupa na grade horária
					int qtdLinhasNaGradeHorariaPorCreditoDaAula = aula.getDuracaoDeUmaAulaEmMinutos()/mdcTemposAula;

					// calcula a linha em que a próxima aula do dia da semana deve ser desenhada
					linhaDaGradeEmQueAulaDeveSerDesenhada += aula.getTotalCreditos()*qtdLinhasNaGradeHorariaPorCreditoDaAula;
				}
			}
		}

		return null;
	}

	private int getWidth( String semana )
	{
		if ( this.qtdColunasPorDiaSemana == null )
		{
			return 100;
		}

		int i = 0;
		if ( semana.equals( "segunda" ) )
		{
			i = 2;
		}

		if ( semana.equals( "terca" ) )
		{
			i = 3;
		}

		if ( semana.equals( "quarta" ) )
		{
			i = 4;
		}

		if ( semana.equals( "quinta" ) )
		{
			i = 5;
		}

		if ( semana.equals( "sexta" ) )
		{
			i = 6;
		}

		if ( semana.equals( "sabado" ) )
		{
			i = 7;
		}

		if ( semana.equals( "domingo" ) )
		{
			i = 1;
		}

		return ( this.qtdColunasPorDiaSemana.get( i ) * 100 );
	}

	private boolean isTatico()
	{
		if ( this.aulasDoBlocoCurricularDTO == null )
		{
			return false;
		}

		if ( this.aulasDoBlocoCurricularDTO.isEmpty() )
		{
			return true;
		}

		AtendimentoRelatorioDTO atm = this.aulasDoBlocoCurricularDTO.get( 0 );

		if ( atm == null )
		{
			return false;
		}

		return ( atm instanceof AtendimentoTaticoDTO );
	}

	public CurriculoDTO getCurriculoDTO()
	{
		return this.curriculoDTO;
	}

	public void setCurriculoDTO( CurriculoDTO curriculoDTO )
	{
		this.curriculoDTO = curriculoDTO;
	}

	public int getPeriodo()
	{
		return this.periodo;
	}

	public void setPeriodo( int periodo )
	{
		this.periodo = periodo;
	}

	public TurnoDTO getTurnoDTO()
	{
		return this.turnoDTO;
	}

	public void setTurnoDTO( TurnoDTO turnoDTO )
	{
		this.turnoDTO = turnoDTO;
	}

	public CampusDTO getCampusDTO()
	{
		return this.campusDTO;
	}

	public void setCampusDTO( CampusDTO campusDTO )
	{
		this.campusDTO = campusDTO;
	}

	public CursoDTO getCursoDTO()
	{
		return this.cursoDTO;
	}

	public void setCursoDTO( CursoDTO cursoDTO )
	{
		this.cursoDTO = cursoDTO;
	}

	public String getCssDisciplina( long id )
	{
		int index = this.disciplinasCores.indexOf( id );

		if ( index < 0 || index > 14 )
		{
			return "corDisciplina14";
		}

		return ( "corDisciplina" + index );
	}

	public void preencheCores()
	{
		Set< Long > set = new HashSet< Long >();

		for ( AtendimentoRelatorioDTO a : this.aulasDoBlocoCurricularDTO )
		{
			set.add( a.getDisciplinaId() );
		}

		this.disciplinasCores.clear();
		this.disciplinasCores.addAll( set );
	}

	public class LinhaDeCredito extends BaseModel {
		private static final long serialVersionUID = 3996652461744817138L;

		private int linhaNaGradeHoraria;

		public LinhaDeCredito(String display, int linhaNaGradeHoraria) {
			this.setDisplay(display);
			this.linhaNaGradeHoraria = linhaNaGradeHoraria;
		}
		
		public int getLinhaNaGradeHoraria() {
			return linhaNaGradeHoraria;
		}

		public String getDisplay()
		{
			return get( "display" );
		}

		public void setDisplay( String value )
		{
			set( "display", value );
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
	}
}
