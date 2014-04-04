package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class HorarioComboBox
	extends ComboBox< HorarioDisponivelCenarioDTO >
{
	private CenarioDTO cenarioDTO;
	private DisciplinaDTO disciplinaDTO;
	private SalaDTO salaDTO;
	private SemanaLetivaDTO semanaLetivaDTO;
	private String diaSemana;

	public HorarioComboBox( CenarioDTO cenario, SalaDTO sala, DisciplinaDTO disciplina, SemanaLetivaDTO semanaLetiva, String semana )
	{
		this.cenarioDTO = cenario;
		this.salaDTO = sala;
		this.disciplinaDTO = disciplina;
		this.semanaLetivaDTO = semanaLetiva;
		this.diaSemana = semana;

		RpcProxy< ListLoadResult< HorarioDisponivelCenarioDTO > > proxy
			= new RpcProxy< ListLoadResult< HorarioDisponivelCenarioDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< HorarioDisponivelCenarioDTO > > callback )
			{
				Services.atendimentos().getHorariosDisponiveisAula(cenarioDTO, salaDTO, disciplinaDTO, semanaLetivaDTO, diaSemana, callback);
			}
		};

		setStore( new ListStore< HorarioDisponivelCenarioDTO >(
			new BaseListLoader< BaseListLoadResult< HorarioDisponivelCenarioDTO > >( proxy ) ) );

		setDisplayField( HorarioDisponivelCenarioDTO.PROPERTY_HORARIO_INICIO_STRING );
		setFieldLabel( "Horário de Início" );
		setEmptyText( "Selecione um horário" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}
	
	public void changeValues(SalaDTO sala, SemanaLetivaDTO semanaLetiva, String semana)
	{
		this.salaDTO = sala;
		this.semanaLetivaDTO = semanaLetiva;
		this.diaSemana = semana;
	}
    
    @Override
    public void onLoad(StoreEvent<HorarioDisponivelCenarioDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem horários cadastrados", null);
        }
    }
}
