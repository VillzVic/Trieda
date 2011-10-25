package com.gapso.web.trieda.shared.services;

import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ErrorsWarningsInputSolverDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "otimizar" )
public interface OtimizarService
	extends RemoteService
{
	Long sendInput( ParametroDTO parametroDTO );
	Boolean isOptimizing( Long round );
	Map< String, List< String > > saveContent( CenarioDTO cenarioDTO, Long round );
	ParametroDTO getParametro( CenarioDTO cenarioDTO );
	ErrorsWarningsInputSolverDTO validaInput( ParametroDTO parametroDTO ) throws Exception;
}
