package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.shared.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ProfessorFormPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		TextField< String > getCpfTextField();
		TextField< String > getNomeTextField();
		TipoContratoComboBox getTipoContratoComboBox();
		NumberField getCargaHorariaMaxNumberField();
		NumberField getCargaHorariaMinNumberField();
		TitulacaoComboBox getTitulacaoComboBox();
		AreaTitulacaoComboBox getAreaTitulacaoComboBox();
		NumberField getCreditoAnteriorNumberField();
		NumberField getValorCreditoNumberField();
		NumberField getMaxDiasSemanaNumberField();
		NumberField getMinCreditosDiaNumberField();
		Button getSalvarButton();
		ProfessorDTO getProfessorDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< ProfessorDTO > gridPanel;
	private Display display;
	private CenarioDTO cenario;

	public ProfessorFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this( instituicaoEnsinoDTO, cenario, display, null );
	}

	public ProfessorFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display,
		SimpleGrid< ProfessorDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.gridPanel = gridPanel;
		this.display = display;
		this.cenario = cenario;

		setListeners();
	}

	private void setListeners()
	{
		display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if ( isValid() )
				{
					final SemanasLetivaServiceAsync service = Services.semanasLetiva();
					service.getList(cenario, new AsyncCallback<ListLoadResult<SemanaLetivaDTO>>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Erro no servidor", null );
						}
						@Override
						public void onSuccess(ListLoadResult<SemanaLetivaDTO> result) {
							if( !checkMinCredMaiorSemanaLetiva(result.getData()) ) {
								final ProfessoresServiceAsync service = Services.professores();
			
								service.save( getDTO(), new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display.getI18nMessages().erroAoSalvar(display.getI18nConstants().professor()),display)
								{
									@Override
									public void onSuccess( Void result )
									{
										display.getSimpleModal().hide();
			
										if ( gridPanel != null )
										{
											gridPanel.updateList();
										}
			
										Info.display( "Salvo", "Item salvo com sucesso!" );
									}
								});
							}
							else
							{
								MessageBox.alert( "ERRO!", "Minimo de creditos diários é maior que a maior semana letiva", null );
							}
						}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!", "Verifique os campos digitados", null );
				}
			}
		});
	}

	private boolean isValid()
	{
		return display.isValid();
	}

	private ProfessorDTO getDTO()
	{
		ProfessorDTO professorDTO = display.getProfessorDTO();

		professorDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );

		professorDTO.setCpf( display.getCpfTextField().getValue() );
		professorDTO.setNome( display.getNomeTextField().getValue() );

		TipoContratoDTO tipoContratoDTO = display.getTipoContratoComboBox().getValue();
		professorDTO.setTipoContratoId( tipoContratoDTO.getId() );
		professorDTO.setTipoContratoString( tipoContratoDTO.getNome() );
		professorDTO.setCargaHorariaMax( display.getCargaHorariaMaxNumberField().getValue().intValue() );
		professorDTO.setCargaHorariaMin( display.getCargaHorariaMinNumberField().getValue().intValue() );

		TitulacaoDTO titulacaoDTO = display.getTitulacaoComboBox().getValue();
		professorDTO.setTitulacaoId( titulacaoDTO.getId() );
		professorDTO.setTitulacaoString( titulacaoDTO.getNome() );

		AreaTitulacaoDTO areaTitulacaoDTO = display.getAreaTitulacaoComboBox().getValue();
		professorDTO.setAreaTitulacaoId( areaTitulacaoDTO == null ? null : areaTitulacaoDTO.getId() );
		professorDTO.setAreaTitulacaoString( areaTitulacaoDTO == null ? null : areaTitulacaoDTO.getCodigo() );
		professorDTO.setCreditoAnterior( display.getCreditoAnteriorNumberField().getValue().intValue() );
		professorDTO.setValorCredito( TriedaUtil.parseTriedaCurrency(
			display.getValorCreditoNumberField().getValue().doubleValue() ) );
		professorDTO.setCenarioId( cenario.getId() );
		professorDTO.setMaxDiasSemana( display.getMaxDiasSemanaNumberField().getValue().intValue() );
		professorDTO.setMinCreditosDia( display.getMinCreditosDiaNumberField().getValue().intValue() );

		return professorDTO;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
	
	private boolean checkMinCredMaiorSemanaLetiva(List<SemanaLetivaDTO> semanasLetiva)
	{
		int maiorSemanaLetiva = 0;
		if(!semanasLetiva.isEmpty()) {
			for(SemanaLetivaDTO semanaLetiva: semanasLetiva) {
				if (semanaLetiva.getHorariosInicioMap().keySet().size() > maiorSemanaLetiva)
					maiorSemanaLetiva = semanaLetiva.getHorariosInicioMap().keySet().size();
			}
			return (display.getMinCreditosDiaNumberField().getValue().intValue() > maiorSemanaLetiva);
		}
		else {
			return false;
		}
	}
}
