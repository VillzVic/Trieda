package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.HashMap;
import java.util.Map;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class RelatorioVisaoAlunoFiltro extends RelatorioVisaoFiltro{
	private static final long serialVersionUID = 1L;
	private String alunoNome;
	private String alunoMatricula;
	
	public ExcelInformationType getExcelType(){
		return ExcelInformationType.RELATORIO_VISAO_ALUNO;
	}
	
	public Map<String, String> getMapStringIds(){
		Map<String, String> mapIds = new HashMap<String, String>();
		
		mapIds.put("alunoNome", alunoNome);
		mapIds.put("alunoMatricula", alunoMatricula);
		
		return mapIds;
	}

	public void addAlunoNomeValueListener(Field<String> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				alunoNome = (String) fe.getValue();
			}
		});
	}
	
	public void addAlunoMatriculaValueListener(Field<String> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				alunoMatricula = (String) fe.getValue();
			}
		});
	}
	
	public String getAlunoNome(){
		return this.alunoNome;
	}

	public void setAlunoNome(String alunoNome){
		this.alunoNome = alunoNome;
	}

	public String getAlunoMatricula(){
		return this.alunoMatricula;
	}

	public void setAlunoMatricula(String alunoMatricula){
		this.alunoMatricula = alunoMatricula;
	}
	
}
