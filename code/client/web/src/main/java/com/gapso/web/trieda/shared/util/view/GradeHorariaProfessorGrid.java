package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
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
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaProfessorGrid extends ContentPanel {

	private Grid<LinhaDeCredito> grid;
	private ListStore<LinhaDeCredito> store;
	private List<AtendimentoOperacionalDTO> atendimentos;
	private ProfessorDTO professorDTO;
	private TurnoDTO turnoDTO;
	private QuickTip quickTip;
	private List<Long> disciplinasCores = new ArrayList<Long>();
	
	private String emptyTextBeforeSearch = "Preencha o filtro acima";
	private String emptyTextAfterSearch = "Não foi encontrado nenhuma Grade Horária para este filtro";
	
	public GradeHorariaProfessorGrid() {
		super(new FitLayout());
		setHeaderVisible(false);
	}

	@Override
	protected void beforeRender() {
		super.beforeRender();
		
		grid = new Grid<LinhaDeCredito>(getListStore(), new ColumnModel(getColumnList()));
		grid.setTrackMouseOver(false);
		grid.setStyleName("GradeHorariaGrid");
		grid.addListener(Events.BeforeSelect, new Listener<GridEvent<LinhaDeCredito>>() {
			@Override
			public void handleEvent(GridEvent<LinhaDeCredito> be) {
				be.setCancelled(true);
			}
		});
		
		grid.getView().setEmptyText(emptyTextBeforeSearch);
		quickTip = new QuickTip(grid);
		quickTip.getToolTipConfig().setDismissDelay(0);
		add(grid);
		
		requestAtendimentos();
	}

	public void requestAtendimentos() {
		if(getProfessorDTO() == null) return;
		grid.mask("Carregando os dados, aguarde alguns instantes", "loading");
		Services.atendimentos().getAtendimentosOperacional(getProfessorDTO(), getTurnoDTO(), new AsyncCallback<List<AtendimentoOperacionalDTO>>(){
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
			}
			@Override
			public void onSuccess(List<AtendimentoOperacionalDTO> result) {
				atendimentos = result;
				preencheCores();
				grid.reconfigure(getListStore(), new ColumnModel(getColumnList()));
				grid.getView().setEmptyText(emptyTextAfterSearch);
				grid.unmask();
			}
		});
	}
	
	public ListStore<LinhaDeCredito> getListStore() {
		if(store == null) {
			store = new ListStore<LinhaDeCredito>();
		} else {
			store.removeAll();
		}
		if(turnoDTO != null) {
			for(Long horarioId : turnoDTO.getHorariosStringMap().keySet()) {
				store.add(new LinhaDeCredito(horarioId, turnoDTO.getHorariosStringMap().get(horarioId)));
			}
		}
		return store;
	}
	
	@Override
	protected void onRender(Element parent, int pos) {
		super.onRender(parent, pos);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		
		addColumn(list, "horario", "Horário");
		addColumn(list, "segunda", "Segunda");
		addColumn(list, "terca", "Terça");
		addColumn(list, "quarta", "Quarta");
		addColumn(list, "quinta", "Quinta");
		addColumn(list, "sexta", "Sexta");
		addColumn(list, "sabado", "Sábado");
		addColumn(list, "domingo", "Domingo");
		return list;
	}
	
	private void addColumn(List<ColumnConfig> list, String id, String name) {
		
		GridCellRenderer<LinhaDeCredito> change = new GridCellRenderer<LinhaDeCredito>() {
			public Html render(LinhaDeCredito model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<LinhaDeCredito> store, Grid<LinhaDeCredito> grid) {
				if(colIndex == 0) return new Html(model.getHorario());
				return content(model.getHorarioId(), rowIndex, colIndex);
			}

			private Html content(long horarioId, int rowIndex, int colIndex) {
				if(atendimentos == null || atendimentos.size() == 0) new Html("");
				
				int semana = -1;
				if(colIndex == 1) semana = 2;
				else if(colIndex == 2) semana = 3;
				else if(colIndex == 3) semana = 4;
				else if(colIndex == 4) semana = 5;
				else if(colIndex == 5) semana = 6;
				else if(colIndex == 6) semana = 7;
				else if(colIndex == 7) semana = 1;
				
				AtendimentoOperacionalDTO atDTO = getAtendimento(horarioId, semana);
				
				if(atDTO == null) return new Html("");
				
				final String title = atDTO.getDisciplinaNome();
				final String contentToolTip = atDTO.getContentToolTipVisaoProfessor();
				
				final Html html = new Html(atDTO.getContentVisaoProfessor()) {
					@Override
					protected void onRender(Element target, int index) {
						super.onRender(target, index);
						target.setAttribute("qtip", contentToolTip);
						target.setAttribute("qtitle", title);
						target.setAttribute("qwidth", "400px");
					}
				};
				html.addStyleName("horario");
				html.addStyleName("c"+(rowIndex + 1));
				html.addStyleName("tc"+atDTO.getTotalLinhas());
				html.addStyleName("s"+atDTO.getSemana());
				html.addStyleName(getCssDisciplina(atDTO.getDisciplinaId()));
				
				return html;
			}
		};
		
		ColumnConfig column = new ColumnConfig(id, name, 100);
		column.setRenderer(change);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		list.add(column);
	}
	
	private AtendimentoOperacionalDTO getAtendimento(Long horarioId, Integer semana) {
		if(atendimentos != null) {
			for(AtendimentoOperacionalDTO at : atendimentos) {
				if(at.getHorarioId().equals(horarioId) && at.getSemana().equals(semana)) {
					return at;
				}
			}
		}
		return null;
	}

	public ProfessorDTO getProfessorDTO() {
		return professorDTO;
	}
	public void setProfessorDTO(ProfessorDTO professorDTO) {
		this.professorDTO = professorDTO;
	}
	
	public TurnoDTO getTurnoDTO() {
		return turnoDTO;
	}
	public void setTurnoDTO(TurnoDTO turnoDTO) {
		this.turnoDTO = turnoDTO;
	}

	public String getCssDisciplina(long id) {
		int index = disciplinasCores.indexOf(id);
		if(index < 0 || index > 14) {
			return "corDisciplina14";
		}
		return "corDisciplina"+index;
	}

	public void preencheCores() {
		Set<Long> set = new HashSet<Long>();
		for(AtendimentoOperacionalDTO a : atendimentos) {
			set.add(a.getDisciplinaId());
		}
		disciplinasCores.clear();
		disciplinasCores.addAll(set);
	}
	
	public class LinhaDeCredito extends BaseModel {

		private static final long serialVersionUID = 3996652461744817138L;
		
		private Long horarioId;
		
		public LinhaDeCredito(Long horarioId, String horarioString) {
			setHorario(horarioString);
			this.horarioId = horarioId;
		}
		
		public Long getHorarioId() {
			return horarioId;
		}
		
		public String getHorario() {
			return get("horario");
		}
		public void setHorario(String value) {
			set("horario", value);
		}
		public Integer getTotalCreditos() {
			return get("totalCreditos");
		}
		public void setTotalCreditos(Integer value) {
			set("totalCreditos", value);
		}
		public String getSegunda() {
			return get("segunda");
		}
		public void setSegunda(String value) {
			set("segunda", value);
		}
		public String getTerca() {
			return get("terca");
		}
		public void setTerca(String value) {
			set("terca", value);
		}
		public String getQuarta() {
			return get("quarta");
		}
		public void setQuarta(String value) {
			set("quarta", value);
		}
		public String getQuinta() {
			return get("quinta");
		}
		public void setQuinta(String value) {
			set("quinta", value);
		}
		public String getSexta() {
			return get("sexta");
		}
		public void setSexta(String value) {
			set("sexta", value);
		}
		public String getSabado() {
			return get("sabado");
		}
		public void setSabado(String value) {
			set("sabado", value);
		}
		public String getDomingo() {
			return get("domingo");
		}
		public void setDomingo(String value) {
			set("domingo", value);
		}
		
	}
	
}
