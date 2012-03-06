package com.gapso.web.trieda.shared.services;

import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ErrorsWarningsInputSolverDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "otimizar" )
public interface OtimizarService extends RemoteService {
	
	/**
	 * A partir dos parâmetros de otimização, verifica a consistência dos dados de entrada antes de enviá-los numa requisição
	 * de otimização.
	 * @param parametroDTO parâmetros da requisição de otimização
	 * @return o status do procedimento de verificação da consistência dos dados de entrada, isto é, se os dados estão válidos ou não e,
	 * em caso negativo, quais são os alertas e os erros existentes.
	 * @throws Exception
	 */
	ErrorsWarningsInputSolverDTO checkInputDataBeforeRequestOptimization(ParametroDTO parametroDTO) throws Exception;
	
	Long sendInput( ParametroDTO parametroDTO );
	Boolean isOptimizing( Long round );
	Map< String, List< String > > saveContent( CenarioDTO cenarioDTO, Long round );
	ParametroDTO getParametro( CenarioDTO cenarioDTO );
}