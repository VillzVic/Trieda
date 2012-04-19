package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.Map;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class RelatorioVisaoCursoFiltro extends RelatorioVisaoFiltro{
	private static final long serialVersionUID = 8367733954996176016L;
	private CursoDTO cursoDTO;
	private CurriculoDTO curriculoDTO;
	private CampusDTO campusDTO;
	private Integer periodo;
	
	public ExcelInformationType getExcelType(){
		return ExcelInformationType.RELATORIO_VISAO_CURSO;
	}
	
	public Map<String, String> getMapStringIds(){
		Map<String, String> mapIds = super.getMapStringIds();
		
		mapIds.put("cursoId", cursoDTO.getId().toString() );
		mapIds.put("curriculoId", curriculoDTO.getId().toString() );
		mapIds.put("campusId", campusDTO.getId().toString() );
		mapIds.put("periodoId", periodo.toString() );
		
		return mapIds;
	}

	public void addCampusValueListener(Field<CampusDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				campusDTO = (CampusDTO) fe.getValue();
			}
		});
	}
	
	public void addCursoValueListener(Field<CursoDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				cursoDTO = (CursoDTO) fe.getValue();
			}
		});
	}
	
	public void addCurriculoValueListener(Field<CurriculoDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				curriculoDTO = (CurriculoDTO) fe.getValue();
			}
		});
	}
	
	public void addPeriodoValueListener(SimpleComboBox<Integer> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			@SuppressWarnings("unchecked")
			public void handleEvent(FieldEvent fe){
				periodo = ((SimpleComboValue<Integer>) fe.getValue()).getValue();
			}
		});
	}

	public CursoDTO getCursoDTO(){
		return this.cursoDTO;
	}

	public void setCursoDTO(CursoDTO cursoDTO){
		this.cursoDTO = cursoDTO;
	}

	public CurriculoDTO getCurriculoDTO(){
		return this.curriculoDTO;
	}

	public void setCurriculoDTO(CurriculoDTO curriculoDTO){
		this.curriculoDTO = curriculoDTO;
	}

	public CampusDTO getCampusDTO(){
		return this.campusDTO;
	}

	public void setCampusDTO(CampusDTO campusDTO){
		this.campusDTO = campusDTO;
	}

	public Integer getPeriodo(){
		return this.periodo;
	}

	public void setPeriodo(Integer periodo){
		this.periodo = periodo;
	}

}
