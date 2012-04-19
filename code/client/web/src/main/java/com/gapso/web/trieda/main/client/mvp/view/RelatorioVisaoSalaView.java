package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoSalaPresenter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SalaComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;

public class RelatorioVisaoSalaView	extends RelatorioVisaoView	implements RelatorioVisaoSalaPresenter.Display{
	private CampusComboBox campusCB;
	private UnidadeComboBox unidadeCB;
	private SalaComboBox salaCB;
	private TurnoComboBox turnoCB;
	private TextField<String> capacidadeTF;
	private TextField<String> tipoSalaTF;
	private RelatorioVisaoSalaFiltro filtro;

	public RelatorioVisaoSalaView(){
		super(new RelatorioVisaoSalaFiltro());
	}

	@Override
	protected String getHeadingPanel(){
		return "Master Data » Grade Horária Visão Sala";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Sala", Resources.DEFAULTS.saidaSala16());
	}
	
	@Override
	protected GradeHorariaSalaGrid createGradeHorariaVisao(){
		return new GradeHorariaSalaGrid();
	}
	
	@Override
	protected void createFilter(){
		Map<LabelAlign, List<Field<?>>> mapLayout = new HashMap<LabelAlign, List<Field<?>>>();
		
		List<Field<?>> leftList = new ArrayList<Field<?>>();
		
		this.campusCB = new CampusComboBox();
		filtro.addCampusValueListener(this.campusCB);
		leftList.add(this.campusCB);
		
		this.unidadeCB = new UnidadeComboBox(this.campusCB);
		filtro.addUnidadeValueListener(this.unidadeCB);
		leftList.add(this.unidadeCB);
		
		List<Field<?>> centerList = new ArrayList<Field<?>>();
		
		this.salaCB = new SalaComboBox(this.unidadeCB);
		filtro.addSalaValueListener(this.salaCB);
		centerList.add(this.salaCB);
		
		this.turnoCB = new TurnoComboBox();
		filtro.addTurnoValueListener(this.turnoCB);
		centerList.add(this.turnoCB);
		
		List<Field<?>> rightList = new ArrayList<Field<?>>();
		
		this.capacidadeTF = new TextField<String>();
		this.capacidadeTF.setFieldLabel("Capacidade");
		this.capacidadeTF.setReadOnly(true);
		rightList.add(this.capacidadeTF);

		this.tipoSalaTF = new TextField< String >();
		this.tipoSalaTF.setFieldLabel( "Tipo" );
		this.tipoSalaTF.setReadOnly( true );
		rightList.add(this.tipoSalaTF);
		
		mapLayout.put(LabelAlign.LEFT, leftList);
		mapLayout.put(LabelAlign.RIGHT, rightList);
		mapLayout.put(LabelAlign.TOP, centerList);
		
		super.createFilter(mapLayout);
	}

	@Override
	public GradeHorariaSalaGrid getGrid(){
		return (GradeHorariaSalaGrid) this.grid;
	}
	
	@Override
	public RelatorioVisaoSalaFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoSalaFiltro) filtro;
	}

	@Override
	public CampusComboBox getCampusComboBox(){
		return this.campusCB;
	}

	@Override
	public UnidadeComboBox getUnidadeComboBox(){
		return this.unidadeCB;
	}

	@Override
	public SalaComboBox getSalaComboBox(){
		return this.salaCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox(){
		return this.turnoCB;
	}

	@Override
	public TextField<String> getCapacidadeTextField(){
		return this.capacidadeTF;
	}

	@Override
	public TextField<String> getTipoTextField(){
		return this.tipoSalaTF;
	}

}
