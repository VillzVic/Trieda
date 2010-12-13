package com.gapso.web.trieda.client.util.view;

import java.util.ArrayList;
import java.util.List;

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
import com.gapso.web.trieda.client.mvp.model.AtendimentoTaticoDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaSalaGrid extends ContentPanel {

	private Grid<LinhaDeCredito> grid;
	private ListStore<LinhaDeCredito> store;
	private List<AtendimentoTaticoDTO> atendimentos;
	private SalaDTO salaDTO;
	private TurnoDTO turnoDTO;
	private QuickTip quickTip;
	
	public GradeHorariaSalaGrid() {
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
		
		grid.getView().setEmptyText("Não houve nenhum resultado");
		quickTip = new QuickTip(grid);
		quickTip.getToolTipConfig().setDismissDelay(0);
		add(grid);
		
		GridDropTarget target = new GridDropTarget(grid) {
			@Override
			protected void onDragDrop(DNDEvent event) { }
		};
		target.addDNDListener(new DNDListener() {
			@Override
			public void dragMove(DNDEvent e) {
				int linha = grid.getView().findRowIndex(e.getDragEvent().getTarget());
				int coluna = grid.getView().findCellIndex(e.getDragEvent().getTarget(), null);
				
				if(linha < 0 || coluna < 1) {
					e.setCancelled(true);
					e.getStatus().setStatus(false);
					return;	
				}
				
				int credito = linha + 1;
				int semana = coluna + 1;
				semana = (semana == 8)? 1 : semana; 
				System.out.println("Linha:  "+ credito);
				System.out.println("Coluna: "+ semana);
				System.out.println("--------------");
				e.setCancelled(false);
				e.getStatus().setStatus(true);
				return;
//				super.dragMove(e);
			}
		});
		requestAtendimentos();
	}

	public void requestAtendimentos() {
		if(getSalaDTO() == null || getTurnoDTO() == null) return;
		AtendimentosServiceAsync service = Services.atendimentos();
		service.getBusca(getSalaDTO(), getTurnoDTO(), new AsyncCallback<List<AtendimentoTaticoDTO>>(){
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
			}
			@Override
			public void onSuccess(List<AtendimentoTaticoDTO> result) {
				atendimentos = result;
				grid.reconfigure(getListStore(), new ColumnModel(getColumnList()));
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
			for(int i = 1; i <= turnoDTO.getMaxCreditos(); i++) {
				store.add(new LinhaDeCredito(i));
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
		
		addColumn(list, "creditos", "Créditos");
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
				if(colIndex == 0) return new Html(String.valueOf(rowIndex + 1));
				if(atendimentos == null || atendimentos.size() == 0) new Html("");
				
				int semana = -1;
				if(colIndex == 1) semana = 2;
				else if(colIndex == 2) semana = 3;
				else if(colIndex == 3) semana = 4;
				else if(colIndex == 4) semana = 5;
				else if(colIndex == 5) semana = 6;
				else if(colIndex == 6) semana = 7;
				else if(colIndex == 7) semana = 1;
				
				AtendimentoTaticoDTO atDTO = getAtendimento(rowIndex + 1, semana);
				
				if(atDTO == null) return new Html("");
				
				final String title = atDTO.getDisciplinaString();
				
				final String contentToolTip = "<b>Curso:</b> "+ atDTO.getCursoString() +"<br />"
					+ "<b>Matriz Curricular:</b> "+ atDTO.getCurriculoString() +"<br />"
					+ "<b>Período:</b> "+ atDTO.getPeriodo() +"<br />"
					+ "<b>Turma:</b> "+ atDTO.getTurma() + "<br />"
					+ "<b>Quantidade:</b> "+ atDTO.getQuantidadeAlunos() +"<br />"
					+ "<b>"+atDTO.getTotalCreditos()+" Crédito(s)</b><br />"
					+ "<b>"+((atDTO.isTeorico())? "Teórico" : "Prático") +"</b><br />";
				
				String content = atDTO.getDisciplinaString() + "<br />";
				content += atDTO.getCursoString() + "<br />";
				content += atDTO.getTurma() + "<br />";
				content += atDTO.getQuantidadeAlunos() +" aluno(s)";
				
				final Html html = new Html(content) {
					@Override
					protected void onRender(Element target, int index) {
						super.onRender(target, index);
						target.setAttribute("qtip", contentToolTip);
						target.setAttribute("qtitle", title);
						target.setAttribute("qwidth", "200px");
					}
				};
				html.addStyleName("horario");
				html.addStyleName("c"+(rowIndex + 1));
				html.addStyleName("tc"+atDTO.getTotalCreditos());
				html.addStyleName("s"+atDTO.getSemana());
				
				new DragSource(html) {
					@Override
					protected void onDragStart(DNDEvent event) {
						event.setData(html);
						event.getStatus().update(El.fly(html.getElement()).cloneNode(true));
						quickTip.hide();
					}
				};
				
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
	
	private AtendimentoTaticoDTO getAtendimento(int credito, int semana) {
		int ocupado = 0;
		if(atendimentos != null) {
			for(AtendimentoTaticoDTO at : atendimentos) {
				if(at.getSemana() == semana) {
	//				if(credito == 1) return at;
					if(credito - 1 == ocupado) {
						return at;
					}
					ocupado += at.getTotalCreditos();
				}
			}
		}
		return null;
	}
	
	public SalaDTO getSalaDTO() {
		return salaDTO;
	}

	public void setSalaDTO(SalaDTO salaDTO) {
		this.salaDTO = salaDTO;
	}

	public TurnoDTO getTurnoDTO() {
		return turnoDTO;
	}

	public void setTurnoDTO(TurnoDTO turnoDTO) {
		this.turnoDTO = turnoDTO;
	}

	public class LinhaDeCredito extends BaseModel {

		private static final long serialVersionUID = 3996652461744817138L;
		
		public LinhaDeCredito(Integer creditos) {
			setCreditos(creditos);
		}
		
		public Integer getCreditos() {
			return get("creditos");
		}
		public void setCreditos(Integer value) {
			set("creditos", value);
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