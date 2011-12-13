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
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaCursoGrid
	extends ContentPanel
{
	private Grid< LinhaDeCredito > grid;
	private ListStore< LinhaDeCredito > store;
	private List< AtendimentoRelatorioDTO > atendimentos;
	private List< Integer > diaSemanaTamanhoList;
	private CurriculoDTO curriculoDTO;
	private SemanaLetivaDTO semanaLetivaDTO;
	private int periodo;
	private TurnoDTO turnoDTO;
	private CampusDTO campusDTO;
	private CursoDTO cursoDTO;
	private QuickTip quickTip;
	private List< Long > disciplinasCores = new ArrayList< Long >();

	private String emptyTextBeforeSearch = "Preencha o filtro acima";
	private String emptyTextAfterSearch = "Não foi encontrado nenhuma Grade Horária para este filtro";

	public GradeHorariaCursoGrid()
	{
		super( new FitLayout() );
		setHeaderVisible( false );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();

		this.grid = new Grid< LinhaDeCredito >( getListStore(),
			new ColumnModel( getColumnList() ) );

		this.grid.setTrackMouseOver( false );
		this.grid.setStyleName( "GradeHorariaGrid" );
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

		SemanasLetivaServiceAsync semanasLetivaService = Services.semanasLetiva();

		semanasLetivaService.findSemanaLetiva( getCurriculoDTO().getSemanaLetivaId(),
			new AsyncCallback< SemanaLetivaDTO >()
		{
			@Override
			public void onFailure( Throwable caught )
			{
				semanaLetivaDTO = null;
			}

			@Override
			public void onSuccess( SemanaLetivaDTO result )
			{
				semanaLetivaDTO = result;
			}
		});

		AtendimentosServiceAsync serviceAtendimentos = Services.atendimentos();

		serviceAtendimentos.getBusca(
			getCurriculoDTO(), getPeriodo(), getTurnoDTO(), getCampusDTO(), getCursoDTO(),
			new AsyncCallback< ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > >()
		{
			@Override
			public void onFailure( Throwable caught )
			{
				MessageBox.alert( "ERRO!",
					"Não foi possível carregar a grade de horários", null );
			}

			@Override
			public void onSuccess(
				ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > result )
			{
				atendimentos = result.getPrimeiro();
				diaSemanaTamanhoList = result.getSegundo();

				preencheCores();
				grid.reconfigure( getListStore(),
					new ColumnModel( getColumnList() ) );

				grid.getView().setEmptyText( emptyTextAfterSearch );
				grid.unmask();
			}
		});
	}

	public ListStore< LinhaDeCredito > getListStore()
	{
		if ( this.store == null )
		{
			this.store = new ListStore< LinhaDeCredito >();
		}
		else
		{
			this.store.removeAll();
		}

		Set< LinhaDeCredito > setLinhaDeCredito
			= new HashSet< LinhaDeCredito >();

		if ( this.semanaLetivaDTO != null )
		{
			if ( isTatico() )
			{
				for ( Integer i = 1; i <= this.semanaLetivaDTO.getMaxCreditos(); i++ )
				{
					setLinhaDeCredito.add( new LinhaDeCredito( i.toString() ) );
				}
			}
			else
			{
				for ( Long horarioId : this.semanaLetivaDTO.getHorariosStringMap().keySet() )
				{
					setLinhaDeCredito.add( new LinhaDeCredito(
						this.semanaLetivaDTO.getHorariosStringMap().get( horarioId ),
						horarioId, this.semanaLetivaDTO.getHorariosInicioMap().get( horarioId ) ) );
				}
			}
		}

		List< LinhaDeCredito > listLinhaDeCredito
			= new ArrayList< LinhaDeCredito >();

		listLinhaDeCredito.addAll( setLinhaDeCredito );
		Collections.sort( listLinhaDeCredito );

		for ( LinhaDeCredito lc : listLinhaDeCredito )
		{
			this.store.add( lc );
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
		List< ColumnConfig > list
			= new ArrayList< ColumnConfig >();

		if ( isTatico() )
		{
			addColumn( list, "display", "Créditos" );
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

		if ( this.diaSemanaTamanhoList == null )
		{
			return count;
		}

		for ( Integer c : this.diaSemanaTamanhoList )
		{
			count += c;
		}

		return count;
	}

	private void addColumn(
		List< ColumnConfig > list, String id, String name )
	{
		GridCellRenderer< LinhaDeCredito > change
			= new GridCellRenderer< LinhaDeCredito >()
		{
			public Html render( LinhaDeCredito model, String property,
				ColumnData config, int rowIndex, int colIndex,
				ListStore< LinhaDeCredito > store, Grid< LinhaDeCredito > grid )
			{
				if ( colIndex == 0 )
				{
					return new Html( model.getDisplay() );
				}

				return content( model, rowIndex, colIndex );
			}

			private Html content( LinhaDeCredito model, int rowIndex, int colIndex )
			{
				if ( colIndex == 0 )
				{
					return new Html( String.valueOf( rowIndex + 1 ) );
				}

				if ( atendimentos == null || atendimentos.size() == 0 )
				{
					new Html( "" );
				}

				int semana = colIndex;
				AtendimentoRelatorioDTO atDTO = null;
				if ( isTatico() )
				{
					atDTO = getAtendimento( rowIndex + 1, semana );
				}
				else
				{
					atDTO = getAtendimento( model.getHorarioId(), semana );
				}

				if ( atDTO == null )
				{
					return new Html( "" );
				}

				final String title = atDTO.getDisciplinaString();
				final String contentToolTip = atDTO.getContentToolTipVisaoCurso(ReportType.WEB);				

				final Html html = new Html( atDTO.getContentVisaoCurso(ReportType.WEB) )
				{
					@Override
					protected void onRender( Element target, int index )
					{
						super.onRender( target, index );

						target.setAttribute( "qtip", contentToolTip );
						target.setAttribute( "qtitle", title );
						target.setAttribute( "qwidth", "400px" );
					}
				};

				html.addStyleName( "horario" );
				html.addStyleName( "c" + ( rowIndex + 1 ) ); // Posiciona na linha ( crédito )
				html.addStyleName( "tc" + atDTO.getTotalCreditos() ); // Altura
				html.addStyleName( "s" + atDTO.getSemana() ); // Posiciona na coluna ( dia semana )
				html.addStyleName( getCssDisciplina( atDTO.getDisciplinaId() ) );

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

	private AtendimentoRelatorioDTO getAtendimento( Long horarioId, int semana )
	{
		int ocupado = 0;

		if ( this.atendimentos != null )
		{
			for ( AtendimentoRelatorioDTO at : this.atendimentos )
			{
				if ( at.getSemana() == semana )
				{
					if ( ( (AtendimentoOperacionalDTO) at ).getHorarioId().equals( horarioId ) )
					{
						return at;
					}

					ocupado += at.getTotalCreditos();
				}
			}
		}

		return null;
	}

	private AtendimentoRelatorioDTO getAtendimento( int credito, int semana )
	{
		int ocupado = 0;

		if ( this.atendimentos != null )
		{
			for ( AtendimentoRelatorioDTO at : this.atendimentos )
			{
				if ( at.getSemana() == semana )
				{
					if ( credito - 1 == ocupado )
					{
						return at;
					}

					ocupado += at.getTotalCreditos();
				}
			}
		}

		return null;
	}

	private int getWidth( String semana )
	{
		if ( this.diaSemanaTamanhoList == null )
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

		return ( this.diaSemanaTamanhoList.get( i ) * 100 );
	}

	private boolean isTatico()
	{
		if ( this.atendimentos == null )
		{
			return false;
		}

		if ( this.atendimentos.isEmpty() )
		{
			return true;
		}

		AtendimentoRelatorioDTO atm = this.atendimentos.get( 0 );

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

		for ( AtendimentoRelatorioDTO a : this.atendimentos )
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

		public LinhaDeCredito( String display,
			Long horarioId, Date horarioInicio )
		{
			this.setDisplay( display );
			this.setHorarioId( horarioId );
			this.setHorarioInicio( horarioInicio );
		}

		public Long getHorarioId()
		{
			return get( "horarioId" );
		}

		public void setHorarioId( Long value )
		{
			set( "horarioId", value );
		}

		public String getDisplay()
		{
			return get( "display" );
		}

		public void setDisplay( String value )
		{
			set( "display", value );
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
