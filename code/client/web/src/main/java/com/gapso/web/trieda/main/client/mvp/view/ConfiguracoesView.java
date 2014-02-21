package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.GXT;
import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.Style.HideMode;
import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.core.XDOM;
import com.extjs.gxt.ui.client.util.Format;
import com.extjs.gxt.ui.client.widget.ComponentHelper;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.WidgetComponent;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.ConfiguracoesPresenter;
import com.gapso.web.trieda.shared.dtos.ParametroConfiguracaoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ConfiguracoesModal;
import com.gapso.web.trieda.shared.util.view.ExportExcelModal;
import com.google.gwt.user.client.Command;
import com.google.gwt.user.client.DeferredCommand;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ConfiguracoesView 
	extends MyComposite
	implements ConfiguracoesPresenter.Display
{

	private ConfiguracoesModal configuracoesModal;
	private FormPanel formPanel;
	private TextField<String> dataSourceTF;
	private TextField<String> urlOtimizacaoTF;
	private TextField<String> nomeOtimizacaoTF;
	private ParametroConfiguracaoDTO parametroConfiguracaoDTO;
	
	public ConfiguracoesView( ParametroConfiguracaoDTO parametroConfiguracaoDTO )
	{
		this.parametroConfiguracaoDTO = parametroConfiguracaoDTO;
		initUI();
	}
	
	private void initUI() 
	{
		String title = "Configurações";
		configuracoesModal = new ConfiguracoesModal(title, Resources.DEFAULTS.exportar16());
		configuracoesModal.setHeight(320);
		configuracoesModal.setWidth(500);
		createForm();
		configuracoesModal.setContent(formPanel);
	}
	
	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setFrame(true);
		formPanel.setHeadingHtml("Configurações do banco e servidor de otimização");
		formPanel.setSize(500, -1);
		formPanel.setButtonAlign(HorizontalAlignment.CENTER);
		formPanel.setScrollMode(Style.Scroll.AUTOY);
		FormLayout formLayout = new FormLayout();
		formLayout.setLabelWidth(120);
		this.formPanel.setLayout(formLayout);
		
		LayoutContainer main = new LayoutContainer();
	    main.setLayout(new FormLayout());
	    
		FieldSet dataBaseFS = new FieldSet();
		dataBaseFS.setWidth(440);
		dataBaseFS.setCollapsible(true);
		dataBaseFS.setHeadingHtml("Configurações do Banco de Dados");
		dataBaseFS.setLayout(new FormLayout());
	    
	    dataSourceTF = new TextField<String>()
	    {
			@Override
			protected void onRender( Element target, int index )
			{
				super.onRender( target, index );
				WidgetComponent info = new WidgetComponent(AbstractImagePrototype.create(Resources.DEFAULTS.info16()).createImage());
				info.setTitle("Nome do data source configurado no servidor de aplicações em que o Trieda está hospedado, por exemplo," +
						" \"DS_TRIEDA\". É através deste data source que a aplicação web do Trieda é capaz" +
						" de criar as conexões com o banco de dados do Trieda.");
				info.setStyleAttribute("position", "absolute");
				info.setStyleAttribute("background-position", "1px 1px");
				
				info.render(el().dom);
				
			}
	    };
	    dataSourceTF.setEmptyText("Nome do Data Source");
	    dataSourceTF.setFieldLabel("Data Source");
	    dataSourceTF.setName("dataSource");
	    dataSourceTF.setValue(parametroConfiguracaoDTO.getDataSource());
	    dataSourceTF.disable();
		formPanel.add(dataSourceTF, formData);
		
		dataBaseFS.add(dataSourceTF);
		main.add(dataBaseFS, new FormData());
		
		FieldSet otimizacaoFS = new FieldSet();
		otimizacaoFS.setWidth(440);
		otimizacaoFS.setCollapsible(true);
		otimizacaoFS.setHeadingHtml("Configurações do Servidor de Otimização");
		otimizacaoFS.setLayout(new FormLayout());
		
	    urlOtimizacaoTF = new TextField<String>()
	    {
			@Override
			protected void onRender( Element target, int index )
			{
				super.onRender( target, index );
				WidgetComponent info = new WidgetComponent(AbstractImagePrototype.create(Resources.DEFAULTS.info16()).createImage());
				info.setTitle("URL do Serviço de Requisições de Otimização: O Servidor de Otimização recebe as requisições de " +
						"otimização oriundas da aplicação web que são gerenciadas por um webservice" +
						" denominado Serviço de Requisições de Otimização. Este serviço, por sua vez," +
						" redireciona uma requisição de otimização para o Resolvedor Matemático " +
						"(um arquivo executável) que a processa e gera uma solução para a mesma, utilizando para" +
						" isso o motor matemático GUROBI. Finalmente, o Serviço de Requisições de Otimização, ao detectar" +
						" a existência de uma solução para a requisição de otimização, envia esta solução de volta para a" +
						" aplicação web que, por sua vez, irá registrá-la no banco de dados. Logo, este parâmetro representa" +
						" a URL através da qual a aplicação web do Trieda se comunica com o Serviço de Requisições de Otimização.");
				info.setStyleAttribute("position", "absolute");
				info.setStyleAttribute("background-position", "1px 1px");
				
				info.render(el().dom);
				
			}
	    };
	    urlOtimizacaoTF.setEmptyText("URL do Serviço de Requisições de Otimização");
	    urlOtimizacaoTF.setFieldLabel("URL de Otimização");
	    urlOtimizacaoTF.setName("urlOtimizacao");
	    urlOtimizacaoTF.setValue(parametroConfiguracaoDTO.getUrlOtimizacao());
		formPanel.add(urlOtimizacaoTF, formData);
		
		nomeOtimizacaoTF = new TextField<String>()
	    {
		@Override
		protected void onRender( Element target, int index )
		{
			super.onRender( target, index );
			WidgetComponent info = new WidgetComponent(AbstractImagePrototype.create(Resources.DEFAULTS.info16()).createImage());
			info.setTitle("O Servidor de Otimização recebe as requisições de otimização oriundas da aplicação web que são gerenciadas" +
					" por um webservice denominado Serviço de Requisições de Otimização. Este serviço, por sua vez, redireciona uma" +
					" requisição de otimização para o Resolvedor Matemático (um arquivo executável) que a processa e gera uma solução" +
					" para a mesma, utilizando para isso o motor matemático GUROBI. Finalmente, o Serviço de Requisições de Otimização," +
					" ao detectar a existência de uma solução para a requisição de otimização, envia esta solução de volta para a aplicação" +
					" web que, por sua vez, irá registrá-la no banco de dados. Logo, este parâmetro representa o nome do arquivo executável" +
					" que representa o Resolvedor Matemático (vale notar que é apenas o nome sem a extensão, por exemplo, \"trieda\" e não \"trieda.exe\").");
			info.setStyleAttribute("position", "absolute");
			info.setStyleAttribute("background-position", "1px 1px");
			
			info.render(el().dom);
			
		}
	    };
	    nomeOtimizacaoTF.setEmptyText("Nome do Resolvedor Matemático");
	    nomeOtimizacaoTF.setFieldLabel("Resolvedor Matemático");
	    nomeOtimizacaoTF.setName("solverName");
	    nomeOtimizacaoTF.setValue(parametroConfiguracaoDTO.getNomeOtimizacao());
		formPanel.add(nomeOtimizacaoTF, formData);
		
		otimizacaoFS.add(urlOtimizacaoTF);
		otimizacaoFS.add(nomeOtimizacaoTF);
		main.add(otimizacaoFS, new FormData());
	    
	    formPanel.add(main, new FormData("100%"));  

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(configuracoesModal.getSalvarBt());

		configuracoesModal.setFocusWidget(dataSourceTF);
		
	}
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public FormPanel getFormPanel() {
		return this.formPanel;
	}
	
	@Override
	public TextField<String> getDataSourceTextField()
	{
		return dataSourceTF;
	}
	
	@Override
	public TextField<String> getUrlOtimizacaoTextField()
	{
		return urlOtimizacaoTF;
	}
	
	@Override
	public TextField<String> getNomeOtimizacaoTextField()
	{
		return nomeOtimizacaoTF;
	}
	
	@Override
	public ConfiguracoesModal getConfiguracoesModal() {
		return this.configuracoesModal;
	}
	
	@Override
	public Button getSalvarBt() {
		return this.configuracoesModal.getSalvarBt();
	}
}
