package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;

public class ProfessoresComJanelasListView
	extends ProfessoresListView
{

	public ProfessoresComJanelasListView(
			CenarioDTO cenarioDTO, RelatorioProfessorFiltro professorFiltro) {
		super(cenarioDTO, professorFiltro);
	}

	@Override
	protected void createPanel() {
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( "Master Data » Professores com Janelas na Grade Horária" );
	}

}