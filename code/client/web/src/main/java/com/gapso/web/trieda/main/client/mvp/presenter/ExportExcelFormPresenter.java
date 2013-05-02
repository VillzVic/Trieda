package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.ExportExcelModal;
import com.google.gwt.user.client.ui.Widget;

public class ExportExcelFormPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getExportarXlsButton();
		Button getExportarXlsxButton();
		boolean isValid();
		ExportExcelModal getExportExcelModal();
		CheckBox getCampiExportExcelCheckBox();
		CheckBox getUnidadesExportExcelCheckBox();
		CheckBox getSalasExportExcelCheckBox();
		CheckBox getCursosExportExcelCheckBox();
		CheckBox getAreasTitulacaoExportExcelCheckBox();
		CheckBox getCursoAreaTitulacaoExportExcelCheckBox();
		CheckBox getDisciplinasExportExcelCheckBox();
		CheckBox getDisciplinasSalasExportExcelCheckBox();
		CheckBox getEquivalenciasExportExcelCheckBox();
		CheckBox getCampiTrabalhoExportExcelCheckBox();
		CheckBox getCurriculosExportExcelCheckBox();
		CheckBox getOfertasDemandasExportExcelCheckBox();
		CheckBox getAlunosExportExcelCheckBox();
		CheckBox getDemandasPorAlunoExportExcelCheckBox();
		CheckBox getProfessoresExportExcelCheckBox();
		CheckBox getDisponibilidadesProfessoresExportExcelCheckBox();
		CheckBox getHabilitacaoProfessoresExportExcelCheckBox();
		CheckBox getResumoCursoExportExcelCheckBox();
		CheckBox getResumoDisciplinaExportExcelCheckBox();
		CheckBox getAtendimentosMatriculaExportExcelCheckBox();
		CheckBox getAtendimentosDisciplinaExportExcelCheckBox();
		CheckBox getAtendimentosFaixaDemandaExportExcelCheckBox();
		CheckBox getRelatorioVisaoSalaExportExcelCheckBox();
		CheckBox getRelatorioVisaoProfessorExportExcelCheckBox();
		CheckBox getRelatorioVisaoAlunoExportExcelCheckBox();
		CheckBox getRelatorioVisaoCursoExportExcelCheckBox();
		CheckBox getAtendimentosPorAlunoExportExcelCheckBox();
		CheckBox getAulasExportExcelCheckBox();
		CheckBox getPercentMestresDoutores();
		FormPanel getFormPanel();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	
	public ExportExcelFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		Display display )
	{
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

		setListeners();
	}
	
	private void setListeners()
	{
		display.getExportarXlsButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					display.getExportExcelModal().hide();
					
					String fileExtension = "xls";
					
					ExcelParametros parametros = new ExcelParametros(
						ExcelInformationType.TUDO, instituicaoEnsinoDTO, fileExtension );

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros, display.getI18nConstants(), display.getI18nMessages(), display.getFormPanel() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				} else {
					MessageBox.alert("ERRO!", "Verifique os campos", null);
				}
			}
		});
		
		display.getExportarXlsxButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					display.getExportExcelModal().hide();
					
					String fileExtension = "xlsx";
					
					ExcelParametros parametros = new ExcelParametros(
						ExcelInformationType.TUDO, instituicaoEnsinoDTO, fileExtension );

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros, display.getI18nConstants(), display.getI18nMessages(), display.getFormPanel() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				} else {
					MessageBox.alert("ERRO!", "Verifique os campos", null);
				}
			}
		});
	}
	
	private boolean isValid()
	{
		return display.isValid();
	}
	
	@Override
	public void go( Widget widget )
	{
		display.getExportExcelModal().show();
	}
}
