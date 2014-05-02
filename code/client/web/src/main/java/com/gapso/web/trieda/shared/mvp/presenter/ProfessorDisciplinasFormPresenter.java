package com.gapso.web.trieda.shared.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.services.ProfessoresDisciplinaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ProfessorDisciplinasFormPresenter
	implements Presenter
{

	public interface Display 
	extends ITriedaI18nGateway {
		Button getSalvarButton();
		ProfessorComboBox getProfessorComboBox();
		ListView< DisciplinaDTO > getNaoSelecionadoList();
		ListView< DisciplinaDTO > getSelecionadoList();
		Button getAdicionaBT();
		Button getRemoveBT();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< ProfessorDisciplinaDTO > gridPanel;
	private Display display;
	private UsuarioDTO usuario;
	private CenarioDTO cenarioDTO;

	public ProfessorDisciplinasFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenarioDTO, UsuarioDTO usuario,
		Display display, SimpleGrid< ProfessorDisciplinaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
		this.usuario = usuario;;
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
		configureProxy();
	}
	
	private void configureProxy() {
		ListStore<DisciplinaDTO> listStoreSelecionados = new ListStore<DisciplinaDTO>();
		display.getSelecionadoList().setStore(listStoreSelecionados);
		
		ListStore<DisciplinaDTO> listStoreNaoSelecionados = new ListStore<DisciplinaDTO>();
		display.getNaoSelecionadoList().setStore(listStoreNaoSelecionados);
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final ProfessoresDisciplinaServiceAsync service = Services.professoresDisciplina();
					service.save(display.getProfessorComboBox().getValue(), getDTO(), new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							gridPanel.updateList();
							Info.display("Salvo", "Item salvo com sucesso!");
						}
					});
				} else {
					MessageBox.alert("ERRO!", "Verifique os campos digitados", null);
				}
			}
		});
		
		display.getProfessorComboBox().addSelectionChangedListener(
			new SelectionChangedListener< ProfessorDTO >()
			{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< ProfessorDTO > se )
			{
				final ProfessorDTO professorDTO = se.getSelectedItem();
				if ( professorDTO != null )
				{
					display.getNaoSelecionadoList().mask(display.getI18nMessages().loading(), "loading");
					display.getSelecionadoList().mask(display.getI18nMessages().loading(), "loading");
					populaListasDisciplinas(professorDTO);
				}
			}
		});
		
		display.getAdicionaBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DisciplinaDTO> campiDTOList = display.getNaoSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getNaoSelecionadoList(), display.getSelecionadoList(), campiDTOList);
			}
		});
		
		display.getRemoveBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DisciplinaDTO> campiDTOList = display.getSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getSelecionadoList(), display.getNaoSelecionadoList(), campiDTOList);
			}
		});
	}
	
	private void transfere(ListView<DisciplinaDTO> origem, ListView<DisciplinaDTO> destino, List<DisciplinaDTO> DisciplinaDTOList) {
		ListStore<DisciplinaDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<DisciplinaDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(DisciplinaDTOList);
		storeVinculadas.sort(DisciplinaDTO.PROPERTY_DISPLAY_TEXT, SortDir.ASC);
		destino.refresh();
		
		for(DisciplinaDTO DisciplinaDTO: DisciplinaDTOList) {
			storeNaoVinculadas.remove(DisciplinaDTO);
		}
		origem.refresh();
	}
	
	private void populaListasDisciplinas(ProfessorDTO professorDTO)
	{
		final ProfessoresDisciplinaServiceAsync service = Services.professoresDisciplina();
		service.getDisciplinasAssociadas(cenarioDTO, professorDTO, new AsyncCallback<ParDTO<List<DisciplinaDTO>, List<DisciplinaDTO>>>() {
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
			}
			@Override
			public void onSuccess(ParDTO<List<DisciplinaDTO>, List<DisciplinaDTO>> result) {
				display.getNaoSelecionadoList().getStore().add(result.getSegundo());
				display.getNaoSelecionadoList().refresh();
				display.getSelecionadoList().getStore().add(result.getPrimeiro());
				display.getSelecionadoList().refresh();
				display.getNaoSelecionadoList().unmask();
				display.getSelecionadoList().unmask();
			}
		});
	}
	
	private boolean isValid() {
		return display.isValid() && (display.getSelecionadoList().getStore().getModels().size() > 0);
	}
	
	private List<ProfessorDisciplinaDTO> getDTO()
	{
		List<ProfessorDisciplinaDTO> dtos = new ArrayList<ProfessorDisciplinaDTO>();
		for (DisciplinaDTO disciplinaDTO : display.getSelecionadoList().getStore().getModels())
		{
			ProfessorDisciplinaDTO unidadeDTO = new ProfessorDisciplinaDTO();

			if ( usuario.isAdministrador() )
			{
				unidadeDTO.setProfessorId( display.getProfessorComboBox().getValue().getId() );
				unidadeDTO.setProfessorString( display.getProfessorComboBox().getValue().getNome() );
				unidadeDTO.setProfessorCpf( display.getProfessorComboBox().getValue().getCpf() );
				unidadeDTO.setDisciplinaId( disciplinaDTO.getId() );
				unidadeDTO.setDisciplinaString( disciplinaDTO.getCodigo() );
				unidadeDTO.setNotaDesempenho( 10 );
			}

			unidadeDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
			unidadeDTO.setPreferencia( 10 );
			
			dtos.add(unidadeDTO);
		}

		return dtos;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
