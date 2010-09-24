package com.gapso.web.trieda.client.util.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.GridEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CellEditor;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.EditorGrid;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.grid.GridSelectionModel;
import com.extjs.gxt.ui.client.widget.grid.HeaderGroupConfig;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.client.mvc.model.UnidadeDeslocamentoModel;

public class DeslocamentoGrid<M extends BaseModel> extends ContentPanel {

	private EditorGrid<M> grid;
	private ListStore<M> store;
	private List<M> models;
	private boolean containsCusto = false;
	
	public DeslocamentoGrid(List<M> deslocamentoUnidadeDTOList) {
		super(new FitLayout());
		models = deslocamentoUnidadeDTOList;
		setHeaderVisible(false);
	}

	protected void createGrid() {
		
		store = new ListStore<M>();
		store.add(models);
		grid = new EditorGrid<M>(store, getColumnModel());
		grid.setBorders(true);
		
		grid.getView().setEmptyText("Não foi encontrado nenhuma unidade por um dos seguintes motivos:<br />&bull; Nenhum campus foi selecionado ao lado;<br />&bull; Não existem nenhuma unidade cadastrada para este campus.");
		
		grid.addListener(Events.BeforeEdit, new Listener<GridEvent<UnidadeDeslocamentoModel>>() {
			public void handleEvent(GridEvent<UnidadeDeslocamentoModel> be) {
				int rowIndex = be.getRowIndex();
				int colIndex = be.getColIndex();
				if(isMeio(rowIndex, colIndex)) {
					be.setCancelled(true);
				}
			}
		});
		add(grid);
	}
	
	@Override
	protected void beforeRender() {
		System.out.println("I beforeRender");
		super.beforeRender();
		createGrid();
		System.out.println("F beforeRender");
	}

	public Grid<M> getGrid() {
		return grid;
	}

	public void updateList(List<M> models) {
		System.out.println("I updateList");
		this.models = models;
		store.removeAll();
		store.add(models);
		grid.reconfigure(store, getColumnModel());
		System.out.println("F updateList");
	}
	
	public ColumnModel getColumnModel() {
		
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		ColumnModel columnModel = null;
			
		if(models != null && !models.isEmpty()) {
			ColumnConfig column = new ColumnConfig("origemString", "Unidades", 120);
			column.setResizable(false);
			column.setMenuDisabled(true);
			column.setSortable(false);
			TextField<String> field = new TextField<String>();
			column.setEditor(new CellEditor(field));
			column.setRenderer(new GridCellRenderer<M>() {
				@Override
				public Object render(M model,
						String property, ColumnData config, int rowIndex,
						int colIndex, ListStore<M> store,
						Grid<M> grid) {
					config.css = "x-grid3-header";
					if(rowIndex == (colIndex - 1)) return "";
					return model.get(property);
				}
			});
			list.add(column);
		
			GridCellRenderer<M> renderer = new GridCellRenderer<M>() {
				@Override
				public Object render(M model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<M> store, Grid<M> grid) {
					if(isMeio(rowIndex, colIndex)) {
						config.style = "background-color: #EBECEE;";
						return "";
					} else {
						config.style = "background-color: #FFFFFF;";
					}
//					return rowIndex+":"+colIndex;
					return null;
				}
			};
		
			for(M model : models) {
				String idColumn = "destinoTempo"+model.get("origemId");
				String stringColumn = "Tempo";
				list.add(createColumnConfig(idColumn, stringColumn, false, renderer));
				
				idColumn = "destinoCusto"+model.get("origemId");
				stringColumn = "Custo";
				if(containsCusto) {
					list.add(createColumnConfig(idColumn, stringColumn, true, renderer));
				}
			}
			columnModel = new ColumnModel(list);
			for(int i = 0; i < models.size(); i++) {
				String stringColumn = models.get(i).get("origemString");
				if(containsCusto) {
					columnModel.addHeaderGroup(0, (i+1)*2-1, new HeaderGroupConfig(stringColumn, 1, 2));
				} else {
					columnModel.addHeaderGroup(0, i+1, new HeaderGroupConfig(stringColumn, 1, 1));
				}
			}
			
		} else {
			columnModel = new ColumnModel(list);
		}

		return columnModel;
	}
	
	public GridSelectionModel<M> getSelectionModel() {
		return grid.getSelectionModel();
	}

	public ListStore<M> getStore() {
		return store;
	}
	
	public void igualarOrigemDestino() {
		for(M itemOrigem : store.getModels()) {
			for(M itemDestino : store.getModels()) {
				itemDestino.set("destinoTempo"+itemOrigem.get("origemId"), itemOrigem.get("destinoTempo"+itemDestino.get("origemId")));
			}
			store.update(itemOrigem);
		}
	}
	
	private boolean isMeio(int rowIndex, int colIndex) {
		return ((containsCusto && rowIndex == ((colIndex-1)/2)) || (!containsCusto && rowIndex == (colIndex-1)));
	}
	
	private ColumnConfig createColumnConfig(String id, String columnString, Boolean allowDecimals, GridCellRenderer<M> renderer) {
		ColumnConfig column = new ColumnConfig(id, columnString, 50);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		NumberField fieldNumber2 = new NumberField();
		fieldNumber2.setAllowDecimals(allowDecimals);
		column.setEditor(new CellEditor(fieldNumber2));
		column.setRenderer(renderer);
		return column;
	}
	
}
