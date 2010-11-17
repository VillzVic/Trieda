package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseTreeModel;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class CenarioPanel extends ContentPanel {
	
	private TreePanel<ModelData> treePanel;
	private TreeStore<ModelData> store = new TreeStore<ModelData>();  

	public CenarioPanel() {
		super(new FitLayout());

		store = new TreeStore<ModelData>();
		
		treePanel = new TreePanel<ModelData>(store);
		treePanel.setDisplayProperty("name");
		treePanel.getStyle().setLeafIcon(AbstractImagePrototype.create(Resources.DEFAULTS.document16())); 
		
		add(treePanel);
		setHeading("Navegação");
	}


	public void addCenario(CenarioDTO cenarioDTO) {
		CenarioTreeModel model = new CenarioTreeModel(cenarioDTO);
		if(store.getAllItems().contains(model)) {
			MessageBox.alert("Cenário já aberto!", "Cenário selecionado já se encontra na árvore de cenário.", null);
		} else {
			store.add(model, true);
		}
	}
	
	private class CenarioTreeModel extends BaseTreeModel {
		
		private static final long serialVersionUID = -975085243596912169L;
		
		private CenarioDTO cenarioDTO;

		public CenarioTreeModel(CenarioDTO cenarioDTO) {
			this.cenarioDTO = cenarioDTO;
			set("name", cenarioDTO.getNome());
			criarEstrutura();
		}
		
		public CenarioDTO getCenarioDTO() {
			return cenarioDTO;
		}
		
		public void criarEstrutura() {
			ItemTreeModel campi = new ItemTreeModel("Campi");
			ItemTreeModel unidades = new ItemTreeModel("Unidades");
			ItemTreeModel salas = new ItemTreeModel("Salas");
			ItemTreeModel[] entrada = {campi, unidades, salas};
			add(new FolderTreeModel("Entrada", entrada));
			
			ItemTreeModel relatorio = new ItemTreeModel("Relatório");
			ItemTreeModel[] saida = {relatorio};
			add(new FolderTreeModel("Saída", saida));
		}

		@Override
		public boolean equals(Object obj) {
			if(!(obj instanceof CenarioTreeModel)) return false;
			return cenarioDTO.getId().equals(((CenarioTreeModel)obj).getCenarioDTO().getId());
		}

	}
	
	private class FolderTreeModel extends BaseTreeModel {

		public FolderTreeModel(String name, BaseTreeModel[] children) {
			set("name", name);
			for (BaseTreeModel bmt : children) {
				add(bmt);
			}
		}
		
	}
	
	private class ItemTreeModel extends BaseTreeModel {
		
		public ItemTreeModel(String name) {
			set("name", name);
		}
		
	}
}
