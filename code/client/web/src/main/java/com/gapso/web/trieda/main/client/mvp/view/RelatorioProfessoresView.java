package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresAreasConhecimentoPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresAtendimentosListPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresBemAlocadosListPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresComJanelasListPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresDeslocamentoCampiListPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresDeslocamentoUnidadesListPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresDisciplinasHabilitadasPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresDisciplinasLecionadasPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresGradeCheiaListPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresJanelasGradePresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresMalAlocadosListPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresTitulacoesPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioProfessoresPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.gapso.web.trieda.shared.util.view.RelatorioView;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioProfessoresView extends RelatorioView
	implements RelatorioProfessoresPresenter.Display
{
	
	RelatorioProfessorFiltro filtro;
	TurnoComboBox turnoCB;
	TitulacaoComboBox titulacaoCB;
	TipoContratoComboBox tipoContratoCB;
	CursoComboBox cursoCB;
	AreaTitulacaoComboBox areaTitulacaoCB;
	
	
	public RelatorioProfessoresView(InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenarioDTO, GTab gTab) {
		super(instituicaoEnsinoDTO, cenarioDTO, gTab);
	}
	
	@Override
	protected String getHeadingPanel() {
		return cenarioDTO.getNome() + " » Relatório de Professores";
	}
	
	@Override
	protected GTabItem createGTabItem() {
		return new GTabItem("Relatório de Professores", Resources.DEFAULTS.professor16());
	}
	
	@Override
	protected void setFilter() {
		this.filtro = new RelatorioProfessorFiltro();
		
		turnoCB = new TurnoComboBox(cenarioDTO);
		titulacaoCB = new TitulacaoComboBox(cenarioDTO);
		tipoContratoCB = new TipoContratoComboBox(cenarioDTO);
		cursoCB = new CursoComboBox(cenarioDTO);
		areaTitulacaoCB = new AreaTitulacaoComboBox();
	}
	
	@Override
	protected List<Field<?>> getFiltersList() {
		List<Field<?>> listFiltro = new ArrayList<Field<?>>();
		listFiltro.add(turnoCB);
		listFiltro.add(titulacaoCB);
		listFiltro.add(tipoContratoCB);
		listFiltro.add(cursoCB);
		listFiltro.add(areaTitulacaoCB);
		
		return listFiltro;
	}
	
	@Override
	public RelatorioProfessorFiltro getProfessorFiltro()
	{
		return filtro;
	}
	
	@Override
	public TurnoComboBox getTurnoComboBox()
	{
		return turnoCB;
	}
	
	@Override
	public CursoComboBox getCursoComboBox()
	{
		return cursoCB;
	}
	
	@Override
	public TitulacaoComboBox getTitulacaoComboBox()
	{
		return titulacaoCB;
	}
	
	@Override
	public TipoContratoComboBox getTipoContratoComboBox()
	{
		return tipoContratoCB;
	}
	
	@Override
	public AreaTitulacaoComboBox getAreaTitulacaoComboBox()
	{
		return areaTitulacaoCB;
	}
	
	@Override
	protected void addListener(Button bt, final RelatorioDTO model)
	{
		switch (model.getButtonIndex())
		{
		case 0:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresJanelasGradePresenter( instituicaoEnsinoDTO,
							cenarioDTO, new ProfessoresJanelasGradeView( cenarioDTO ) );
					presenter.go( gTab );
				}
			});
			break;
		case 1:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresDisciplinasHabilitadasPresenter( instituicaoEnsinoDTO,
							cenarioDTO, new ProfessoresDisciplinasHabilitadasView( cenarioDTO ) );
					presenter.go( gTab );
				}
			});
			break;
		case 2:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresDisciplinasLecionadasPresenter( instituicaoEnsinoDTO,
							cenarioDTO, new ProfessoresDisciplinasLecionadasView( cenarioDTO ) );
					presenter.go( gTab );
				}
			});
			break;
		case 3:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresTitulacoesPresenter( instituicaoEnsinoDTO,
							cenarioDTO, new ProfessoresTitulacoesView( cenarioDTO ) );
					presenter.go( gTab );
				}
			});
			break;
		case 4:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresAreasConhecimentoPresenter( instituicaoEnsinoDTO,
							cenarioDTO, new ProfessoresAreasConhecimentoView( cenarioDTO ) );
					presenter.go( gTab );
				}
			});
			break;
		case 5:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresAtendimentosListPresenter( instituicaoEnsinoDTO,
							model.getCampusId(),
							cenarioDTO, new ProfessoresAtendimentosListView( cenarioDTO, getProfessorFiltro() ) );
					presenter.go( gTab );
				}
			});
			break;
		case 6:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresGradeCheiaListPresenter( instituicaoEnsinoDTO,
							model.getCampusId(),
							cenarioDTO, new ProfessoresGradeCheiaListView( cenarioDTO, getProfessorFiltro() ) );
					presenter.go( gTab );
				}
			});
			break;
		case 7:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresBemAlocadosListPresenter( instituicaoEnsinoDTO,
							model.getCampusId(),
							cenarioDTO, new ProfessoresBemAlocadosListView( cenarioDTO, getProfessorFiltro() ) );
					presenter.go( gTab );
				}
			});
			break;
		case 8:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresMalAlocadosListPresenter( instituicaoEnsinoDTO,
							model.getCampusId(),
							cenarioDTO, new ProfessoresMalAlocadosListView( cenarioDTO, getProfessorFiltro() ) );
					presenter.go( gTab );
				}
			});
			break;
		case 9:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresComJanelasListPresenter( instituicaoEnsinoDTO,
							model.getCampusId(),
							cenarioDTO, new ProfessoresComJanelasListView( cenarioDTO, getProfessorFiltro() ) );
					presenter.go( gTab );
				}
			});
			break;
		case 10:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresDeslocamentoUnidadesListPresenter( instituicaoEnsinoDTO,
							model.getCampusId(),
							cenarioDTO, new ProfessoresDeslocamentoUnidadesListView( cenarioDTO, getProfessorFiltro() ) );
					presenter.go( gTab );
				}
			});
			break;
		case 11:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new ProfessoresDeslocamentoCampiListPresenter( instituicaoEnsinoDTO,
							model.getCampusId(),
							cenarioDTO, new ProfessoresDeslocamentoCampiListView( cenarioDTO, getProfessorFiltro() ) );
					presenter.go( gTab );
				}
			});
			break;
		}
	}
}
