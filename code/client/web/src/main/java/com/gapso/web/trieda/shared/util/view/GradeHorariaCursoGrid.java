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
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaCursoGrid extends ContentPanel {

	private Grid<LinhaDeCredito> grid;
	private ListStore<LinhaDeCredito> store;
	private List<AtendimentoRelatorioDTO> atendimentos;
	private List<Integer> diaSemanaTamanhoList;
	private CurriculoDTO curriculoDTO;
	private int periodo;
	private TurnoDTO turnoDTO;
	private CampusDTO campusDTO;
	private QuickTip quickTip;
	private List<Long> disciplinasCores = new ArrayList<Long>();
	
	private String emptyTextBeforeSearch = "Preencha o filtro acima";
	private String emptyTextAfterSearch = "Não foi encontrado nenhuma Grade Horária para este filtro";
	
	public GradeHorariaCursoGrid() {
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
				e.setCancelled(false);
				e.getStatus().setStatus(true);
				return;
			}
		});
		requestAtendimentos();
	}

	public void requestAtendimentos() {
		if(getCurriculoDTO() == null || getTurnoDTO() == null  || getCampusDTO() == null || getPeriodo() <= 0) return;
		grid.mask("Carregando os dados, aguarde alguns instantes", "loading");
		AtendimentosServiceAsync service = Services.atendimentos();
		service.getBusca(getCurriculoDTO(), getPeriodo(), getTurnoDTO(), getCampusDTO(), new AsyncCallback<ParDTO<List<AtendimentoRelatorioDTO>, List<Integer>>>(){
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
			}
			@Override
			public void onSuccess(ParDTO<List<AtendimentoRelatorioDTO>, List<Integer>> result) {
				atendimentos = result.getPrimeiro();
				diaSemanaTamanhoList = result.getSegundo();
				
				preencheCores();
				//coletaAtendimentosParalelos();
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
			for(int i = 1; i <= turnoDTO.getMaxCreditos() * 5; i++) {
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
		addColumn(list, "extra1", "");
		addColumn(list, "extra2", "");
		addColumn(list, "extra3", "");
		addColumn(list, "extra4", "");
		
		return list;
	}
	
	private void addColumn(List<ColumnConfig> list, String id, String name) {
		
		GridCellRenderer<LinhaDeCredito> change = new GridCellRenderer<LinhaDeCredito>() {
			public Html render(LinhaDeCredito model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<LinhaDeCredito> store, Grid<LinhaDeCredito> grid) {
				return content(rowIndex, colIndex);
			}

			private Html content(int rowIndex, int colIndex) {
				if(colIndex == 0) return new Html(String.valueOf(rowIndex + 1));
				if(atendimentos == null || atendimentos.size() == 0) new Html("");
				
				int semana = colIndex;
				
				AtendimentoRelatorioDTO atDTO = getAtendimento(rowIndex + 1, semana);
				
				if(atDTO == null) return new Html("");
				
				final String title = atDTO.getDisciplinaString();
				
				String contentToolTipAux = "<b>Nome:</b> "+ atDTO.getDisciplinaNome() +"<br />"
					+ "<b>Sala:</b> "+ atDTO.getSalaString() +"<br />"
					+ "<b>Turma:</b> "+ atDTO.getTurma() + "<br />"
					+ "<b>"+atDTO.getQuantidadeAlunos()+" alunos(s)</b><br />"
					+ "<b>Tipo Crédito:</b> "+((atDTO.isTeorico())? "Teórico" : "Prático") +"<br />"
					+ "<b>Créditos:</b> "+atDTO.getTotalCreditos()+" de "+atDTO.getTotalCreditoDisciplina()+"<br />";
				final String contentToolTip = contentToolTipAux;
				
				
				String content = atDTO.getDisciplinaString() + "<br />";
				
				content += TriedaUtil.truncate(atDTO.getDisciplinaNome(),12);
				content += "<br />";
				
				content += atDTO.getUnidadeString();
				content += "<br />";
				
				content += atDTO.getSalaString();
				content += "<br />";
				
				content += "Turma " + atDTO.getTurma();
				content += "<br />";
				
				final Html html = new Html(content) {
					@Override
					protected void onRender(Element target, int index) {
						super.onRender(target, index);
						target.setAttribute("qtip", contentToolTip);
						target.setAttribute("qtitle", title);
						target.setAttribute("qwidth", "400px");
					}
				};
				html.addStyleName("horario");
				html.addStyleName("c"+(rowIndex + 1)); // posiciona na linha (credito)
				html.addStyleName("tc"+atDTO.getTotalCreditos()); // altura
				html.addStyleName("s"+atDTO.getSemana()); // posiciona na columa (dia semana)
				html.addStyleName(getCssDisciplina(atDTO.getDisciplinaId()));
				
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
		
		ColumnConfig column = new ColumnConfig(id, name, getWidth(id));
		column.setRenderer(change);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		list.add(column);
	}
	
	private AtendimentoRelatorioDTO getAtendimento(int credito, int semana) {
		int ocupado = 0;
		if(atendimentos != null) {
			for(AtendimentoRelatorioDTO at : atendimentos) {
				//if(!atendimentosParalelos.containsKey(at)) continue;
				if(at.getSemana() == semana) {
					if(credito - 1 == ocupado) {
						return at;
					}
					ocupado += at.getTotalCreditos();
				}
			}
		}
		return null;
	}
	
	private int getWidth(String semana) {
		if(diaSemanaTamanhoList == null) return 100; 
		int i = 0;
		if(semana.equals("segunda")) { i = 2; }
		if(semana.equals("terca"))   { i = 3; }
		if(semana.equals("quarta"))  { i = 4; }
		if(semana.equals("quinta"))  { i = 5; }
		if(semana.equals("sexta"))   { i = 6; }
		if(semana.equals("sabado"))  { i = 7; }
		if(semana.equals("domingo")) { i = 1; }
		return diaSemanaTamanhoList.get(i) * 100;
	}
	
	
	public CurriculoDTO getCurriculoDTO() {
		return curriculoDTO;
	}
	public void setCurriculoDTO(CurriculoDTO curriculoDTO) {
		this.curriculoDTO = curriculoDTO;
	}
	
	public int getPeriodo() {
		return periodo;
	}
	public void setPeriodo(int periodo) {
		this.periodo = periodo;
	}

	public TurnoDTO getTurnoDTO() {
		return turnoDTO;
	}
	public void setTurnoDTO(TurnoDTO turnoDTO) {
		this.turnoDTO = turnoDTO;
	}
	
	public CampusDTO getCampusDTO() {
		return campusDTO;
	}
	public void setCampusDTO(CampusDTO campusDTO) {
		this.campusDTO = campusDTO;
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
		for(AtendimentoRelatorioDTO a : atendimentos) {
			set.add(a.getDisciplinaId());
		}
		disciplinasCores.clear();
		disciplinasCores.addAll(set);
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
