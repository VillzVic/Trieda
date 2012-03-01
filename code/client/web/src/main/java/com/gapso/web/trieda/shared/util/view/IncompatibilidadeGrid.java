package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.grid.CellEditor;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.EditorGrid;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.shared.dtos.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.shared.util.resources.Resources;

public class IncompatibilidadeGrid extends ContentPanel {

	private EditorGrid<BaseModel> grid;
	private ListStore<BaseModel> store;
	private List<DisciplinaIncompativelDTO> models = new ArrayList<DisciplinaIncompativelDTO>();
	private GridCellRenderer<BaseModel> buttonRenderer;
	private List<String> stringColumns = new ArrayList<String>();
	
	public IncompatibilidadeGrid()
	{
		super( new FitLayout() );
		setHeaderVisible( false );
	}

	protected void createGrid()
	{
		store = new ListStore< BaseModel >();
		store.add( models );
		grid = new EditorGrid< BaseModel >( store, getColumnModel() );
		grid.setBorders( true );

		buttonRenderer = new GridCellRenderer< BaseModel >() {
			@Override
			public Object render( BaseModel model, String property, ColumnData config,
				final int row, final int col, ListStore< BaseModel > store, Grid< BaseModel > grid )
			{
				if ( col == 0 )
				{
					return stringColumns.get( row );
				}

				if ( isInferior( row, col ) )
				{
					config.style = "background-color: #EEEFFF; height: 28px;";
					return "";
				}
				
				DisciplinaIncompativelDTO dto = getModel(row,col);
				if (dto != null) {
					ToggleImageButton tb = new ToggleImageButton( getModel( row, col ).getIncompativel(),
						Resources.DEFAULTS.incompativel(), Resources.DEFAULTS.compativel() );
	
					tb.setWidth( 69 );
					tb.addSelectionListener( new SelectionListener< ButtonEvent >()
					{
						@Override
						public void componentSelected( ButtonEvent ce )
						{
							ToggleImageButton tib = (ToggleImageButton) ce.getComponent();
							getModel( row, col ).setIncompativel( tib.isPressed() );
						}
					});
	
					return tb;
				} else {
					return "";
				}
			}
		};

		add( grid );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();
		createGrid();
	}

	public void updateList( List< DisciplinaIncompativelDTO > models )
	{
		this.models = ( ( models != null )? models : new ArrayList< DisciplinaIncompativelDTO >() );
		store.removeAll();
		stringColumns.clear();

		Set<String> disciplinasStr = new HashSet<String>();
		for ( DisciplinaIncompativelDTO di : models ) {
			disciplinasStr.add(di.getDisciplina1String());
			disciplinasStr.add(di.getDisciplina2String());
		}
		
		stringColumns.addAll(disciplinasStr);

		List< BaseModel > modelsFake = new ArrayList<BaseModel>();
		modelsFake.add(new BaseModel());
		for(int i = 0; i < stringColumns.size() - 1; i++) {
			modelsFake.add(new BaseModel());
		}
		store.add(modelsFake);
		
		grid.reconfigure(store, getColumnModel());
	}
	
	public ColumnModel getColumnModel() {
		
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(createColumnConfig("", true));
		for(String title : stringColumns) {
			list.add(createColumnConfig(title, false));
		}
		return new ColumnModel(list);
	}

	private CheckColumnConfig createColumnConfig(String title, boolean firstCol) {
		CheckColumnConfig column = new CheckColumnConfig("", title, 150);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setEditor(new CellEditor(new CheckBox()));
		column.setRenderer(buttonRenderer);
		if(firstCol) {
			column.setStyle("background-color: #EBECEE;");
		}
		return column;
	}
	
	public List<DisciplinaIncompativelDTO> getModels() {
		return models;
	}
	
	private DisciplinaIncompativelDTO getModel(int row, int col) {
		if (row < stringColumns.size() && (col-1) < stringColumns.size()) {
			String rowString = stringColumns.get(row);
			String columnString = stringColumns.get(col - 1);
			for(DisciplinaIncompativelDTO model : models) {
				if((model.getDisciplina1String().equals(rowString) && model.getDisciplina2String().equals(columnString)) ||
				   (model.getDisciplina2String().equals(rowString) && model.getDisciplina1String().equals(columnString))) {
					return model;
				}
			}
		}
		return null;
	}

	private boolean isInferior(int row, int col) {
		return (row >= col-1);
	}
	
}
