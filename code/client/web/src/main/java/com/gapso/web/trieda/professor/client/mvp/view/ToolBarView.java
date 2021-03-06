package com.gapso.web.trieda.professor.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.professor.client.mvp.presenter.ToolBarPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ToolBarView extends MyComposite
	implements ToolBarPresenter.Display
{
	private LayoutContainer container;
	private ToolBar toolBar;
	private Button professoresCampusListprofessoresBt;
	private Button professoresDisciplinaListprofessoresBt;
	private Button disponibilidadesProfessorBt;
	private Button relatorioVisaoProfessorBt;

	public ToolBarView()
	{
		initUI();
	}

	private void initUI()
	{
		toolBar = new ToolBar();
		createButtons();

		container = new LayoutContainer( new BorderLayout() );
		container.setHeight( 65 );

		BorderLayoutData centerData
			= new BorderLayoutData( LayoutRegion.CENTER );

		container.add( toolBar, centerData );

		initComponent( container );
	}

	private void createButtons()
	{
		professoresCampusListprofessoresBt = createButton( "Campi de<br />Trabalho",
			"Campi de Trabalho", Resources.DEFAULTS.campiTrabalho24() );
		toolBar.add( professoresCampusListprofessoresBt );

		professoresDisciplinaListprofessoresBt = createButton( "Habilitação<br />do Professor",
			"Habilitação do Professor", Resources.DEFAULTS.habilitacaoProfessor24() );
		toolBar.add( professoresDisciplinaListprofessoresBt );
		
		disponibilidadesProfessorBt = createButton( "Disponibilidade<br />do Professor",
			"Disponibilidade do Professor", Resources.DEFAULTS.semanaLetiva24() );
		toolBar.add( disponibilidadesProfessorBt );
		
		relatorioVisaoProfessorBt = createButton( "Grade Horária<br />Visão Professor",
			"Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor24() );
		toolBar.add( relatorioVisaoProfessorBt );		
	}

	private Button createButton( String text, String toolTip, ImageResource icon )
	{
		Button bt = new Button( text, AbstractImagePrototype.create( icon ) );

		bt.setToolTip( toolTip );
		bt.setScale( ButtonScale.MEDIUM );
		bt.setIconAlign( IconAlign.TOP );
		bt.setArrowAlign( ButtonArrowAlign.BOTTOM );
		bt.setHeight( 60 );

		return bt;
	}

	@Override
	public Button getProfessoresCampusListprofessoresBt()
	{
		return professoresCampusListprofessoresBt;
	}

	@Override
	public Button getRelatorioVisaoProfessorButton()
	{
		return relatorioVisaoProfessorBt;
	}

	@Override
	public Button getProfessoresDisciplinaListProfessoresButton()
	{
		return professoresDisciplinaListprofessoresBt;
	}

	@Override
	public Button getDisponibilidadeProfessorButton()
	{
		return disponibilidadesProfessorBt;
	}
}
