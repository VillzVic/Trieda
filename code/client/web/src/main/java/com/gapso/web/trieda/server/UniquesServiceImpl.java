package com.gapso.web.trieda.server;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.services.UniquesService;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;

/**
 * The server side implementation of the RPC service.
 */
public class UniquesServiceImpl extends RemoteService implements UniquesService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public Boolean existUnique(CenarioDTO cenarioDTO, String value, String uniqueDomain) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		Boolean exist = true;
		UniqueDomain uniqueDomainEnum = UniqueDomain.valueOf(uniqueDomain);
		switch (uniqueDomainEnum) {
			case CAMPI: exist = checkCampus(cenario, value); break;
			case UNIDADE: exist = checkUnidade(cenario, value); break;
			case SALA: exist = checkSala(cenario, value); break;
			case CURSO: exist = checkCurso(cenario, value); break;
		}
		return exist;
	}
	
	private Boolean checkCampus(Cenario cenario, String value) {
		return Campus.checkCodigoUnique(cenario, value);
	}
	private Boolean checkUnidade(Cenario cenario, String value) {
		return Unidade.checkCodigoUnique(cenario, value);
	}
	private Boolean checkSala(Cenario cenario, String value) {
		return Sala.checkCodigoUnique(cenario, value);
	}
	private Boolean checkCurso(Cenario cenario, String value) {
		return Curso.checkCodigoUnique(cenario, value);
	}

}
