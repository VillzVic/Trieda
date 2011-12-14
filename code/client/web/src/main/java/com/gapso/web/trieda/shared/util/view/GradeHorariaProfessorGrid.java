package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.data.BaseModel;
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
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaProfessorGrid
	extends ContentPanel
{
	private Grid< LinhaDeCredito > grid;
	private ListStore< LinhaDeCredito > store;
	private List< AtendimentoOperacionalDTO > atendimentos;
	private ProfessorDTO professorDTO;
	private ProfessorVirtualDTO professorVirtualDTO;
	private TurnoDTO turnoDTO;
	private SemanaLetivaDTO semanaLetivaDTO;	
	private QuickTip quickTip;
	private List< Long > disciplinasCores = new ArrayList< Long >();

	private String emptyTextBeforeSearch = "Preencha o filtro acima";
	private String emptyTextAfterSearch = "Não foi encontrado nenhuma Grade Horária para este filtro";
	private boolean isVisaoProfessor;

	public GradeHorariaProfessorGrid( boolean isVisaoProfessor )
	{
		super( new FitLayout() );

		this.isVisaoProfessor = isVisaoProfessor; 
		this.setHeaderVisible( false );
	}

	public boolean isVisaoProfessor()
	{
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor( boolean isVisaoProfessor )
	{
		this.isVisaoProfessor = isVisaoProfessor;
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
		this.add( this.grid );

		requestAtendimentos();
	}

	public void requestAtendimentos()
	{
		if ( getTurnoDTO() == null || getSemanaLetivaDTO() == null
			|| ( getProfessorDTO() == null && getProfessorVirtualDTO() == null ) )
		{
			return;
		}

		this.grid.mask( "Carregando os dados, " +
			"aguarde alguns instantes", "loading" );

		Services.atendimentos().getAtendimentosOperacional( getProfessorDTO(),
			getProfessorVirtualDTO(), getTurnoDTO(), this.isVisaoProfessor(), getSemanaLetivaDTO(),
			new AsyncCallback< List< AtendimentoOperacionalDTO > >()
			{
				@Override
				public void onFailure( Throwable caught )
				{
					MessageBox.alert( "ERRO!",
						"Não foi possível carregar a grade de horários", null );
				}

				@Override
				public void onSuccess( List< AtendimentoOperacionalDTO > result )
				{
					atendimentos = result;
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

		if ( this.turnoDTO != null )
		{
			for ( Long horarioId : this.turnoDTO.getHorariosStringMap().keySet() )
			{
				String horarioStr = this.turnoDTO.getHorariosStringMap().get( horarioId ); 
				Date horarioInicio = this.turnoDTO.getHorariosInicioMap().get( horarioId );

				setLinhaDeCredito.add( new LinhaDeCredito(
					horarioId, horarioStr, horarioInicio ) );
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
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		addColumn( list, "horario", "Horário" );
		addColumn( list, "segunda", "Segunda" );
		addColumn( list, "terca", "Terça" );
		addColumn( list, "quarta", "Quarta" );
		addColumn( list, "quinta", "Quinta" );
		addColumn( list, "sexta", "Sexta" );
		addColumn( list, "sabado", "Sábado" );
		addColumn( list, "domingo", "Domingo" );

		return list;
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
					return new Html( model.getHorario() );
				}

				return content( model.getHorarioId(), rowIndex, colIndex );
			}

			private Html content( long horarioId, int rowIndex, int colIndex )
			{
				if ( atendimentos == null
					|| atendimentos.size() == 0 )
				{
					new Html( "" );
				}

				int semana = -1;
				if ( colIndex == 1 )
				{
					semana = 2;
				}
				else if ( colIndex == 2 ) 
				{
					semana = 3;
				}
				else if ( colIndex == 3 )
				{
					semana = 4;
				}
				else if ( colIndex == 4 )
				{
					semana = 5;
				}
				else if ( colIndex == 5 )
				{
					semana = 6;
				}
				else if ( colIndex == 6 )
				{
					semana = 7;
				}
				else if ( colIndex == 7 )
				{
					semana = 1;
				}

				AtendimentoOperacionalDTO atDTO
					= getAtendimento( horarioId, semana );

				if ( atDTO == null )
				{
					return new Html( "" );
				}

				final String title = atDTO.getDisciplinaNome();
				final String contentToolTip = atDTO.getContentToolTipVisaoProfessor(ReportType.WEB);

				final Html html = new Html( atDTO.getContentVisaoProfessor(ReportType.WEB) )
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
				html.addStyleName( "c" + ( rowIndex + 1 ) );
				html.addStyleName( "tc" + atDTO.getTotalCreditos() );
				html.addStyleName( "s" + atDTO.getSemana() );
				html.addStyleName( getCssDisciplina( atDTO.getDisciplinaId() ) );

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

	private AtendimentoOperacionalDTO getAtendimento(
		Long horarioId, Integer semana )
	{
		if ( this.atendimentos != null )
		{
			for ( AtendimentoOperacionalDTO at : this.atendimentos )
			{
				if ( at.getHorarioId().equals( horarioId )
					&& at.getSemana().equals( semana ) )
				{
					return at;
				}
			}
		}

		return null;
	}

	public ProfessorDTO getProfessorDTO()
	{
		return this.professorDTO;
	}

	public void setProfessorDTO(
		ProfessorDTO professorDTO )
	{
		this.professorDTO = professorDTO;
	}

	public ProfessorVirtualDTO getProfessorVirtualDTO()
	{
		return this.professorVirtualDTO;
	}

	public void setProfessorVirtualDTO(
		ProfessorVirtualDTO professorVirtualDTO )
	{
		this.professorVirtualDTO = professorVirtualDTO;
	}

	public TurnoDTO getTurnoDTO()
	{
		return this.turnoDTO;
	}

	public void setTurnoDTO( TurnoDTO turnoDTO )
	{
		this.turnoDTO = turnoDTO;
	}

	public SemanaLetivaDTO getSemanaLetivaDTO()
	{
		return this.semanaLetivaDTO;
	}

	public void setSemanaLetivaDTO(
		SemanaLetivaDTO semanaLetivaDTO )
	{
		this.semanaLetivaDTO = semanaLetivaDTO;
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

		for ( AtendimentoOperacionalDTO a : this.atendimentos )
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

		private Long horarioId;
		private Date horarioInicio;

		public LinhaDeCredito( Long horarioId,
			String horarioString, Date horarioInicio )
		{
			this.setHorario( horarioString );

			this.horarioId = horarioId;
			this.horarioInicio = horarioInicio;
		}

		public Date getHorarioInicio()
		{
			return this.horarioInicio;
		}

		public void setHorarioInicio( Date horarioInicio )
		{
			this.horarioInicio = horarioInicio;
		}

		public Long getHorarioId()
		{
			return this.horarioId;
		}

		public String getHorario()
		{
			return get( "horario" );
		}

		public void setHorario( String value )
		{
			set( "horario", value );
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
				// TODO -- comparar strings de horários de
				// início e fim, no formato : HH:mm / HH:mm
				String str1 = ( this.getHorario() == null ? "" : this.getHorario() );
				String str2 = ( o.getHorario() == null ? "" : o.getHorario() );

				return str1.compareTo( str2 );
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
