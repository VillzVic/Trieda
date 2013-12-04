package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;

public class ProfessoresDeslocamentoUnidadesListView
	extends ProfessoresListView
{

	public ProfessoresDeslocamentoUnidadesListView(
			RelatorioProfessorFiltro professorFiltro) {
		super(professorFiltro);
	}

	@Override
	protected void createPanel() {
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( "Master Data » Professores com Deslocamento Entre Unidades" );
	}

}