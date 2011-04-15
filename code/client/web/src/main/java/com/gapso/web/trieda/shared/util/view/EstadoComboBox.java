package com.gapso.web.trieda.shared.util.view;

import java.util.Arrays;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.ListModelPropertyEditor;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.util.view.EstadoComboBox.Estado;

public class EstadoComboBox extends SimpleComboBox<Estado> {

	public EstadoComboBox() {
		List<Estado> enumList = Arrays.asList(Estado.values());
		add(enumList);
		setSimpleValue(enumList.get(0));
		setLazyRender(false);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	
		setPropertyEditor(new ListModelPropertyEditor<SimpleComboValue<Estado>>() {
			@Override
			public String getStringValue(SimpleComboValue<Estado> value) {
				return value.getValue().getNome();
			}
		});
		setDisplayField("value");

	}

	public void setValue(String valueString) {
		if(valueString == null) setValueField(null); 
		for(SimpleComboValue<Estado> value : getStore().getModels()) {
			if(value.getValue().name().equals(valueString)) {
				super.setValue(value);
			}
		}
	}
	
	public enum Estado {
	    AC("Acre"),
	    AL("Alagoas"),
	    AP("Amapá"),
	    AM("Amazonas"),
	    BA("Bahia"),
	    CE("Ceará"),
	    DF("Distrito Federal"),
	    GO("Goias"),
	    ES("Espírito Santos"),
	    MA("Maranhão"),
	    MT("Mato Grosso"),
	    MS("Mato Grosso do Sul"),
	    MG("Minas Gerais"),
	    PA("Pará"),
	    PB("Paraíba"),
	    PR("Paraná"),
	    PE("Pernanbuco"),
	    PI("Piauí"),
	    RJ("Rio de Janeiro"),
	    RN("Rio Grande do Norte"),
	    RS("Rio Grande do Sul"),
	    RO("Rondônia"),
	    RR("Roraima"),
	    SP("São Paulo"),
	    SC("Santa Catarina"),
	    SE("Sergipe"),
	    TO("Tocantins");
	    
	    private String nome;
	    
	   Estado(String nome) {
	    	this.nome = nome;
	    }

	    public String getNome() {
	    	return nome;
	    }
	    
	    @Override
	    public String toString() {
	    	return getNome();
	    }
	    
	}
	
}
