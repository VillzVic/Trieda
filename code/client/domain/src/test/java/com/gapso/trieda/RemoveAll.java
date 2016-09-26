package com.gapso.trieda;

import java.io.FileOutputStream;

import javax.sql.DataSource;

import org.dbunit.database.DatabaseDataSourceConnection;
import org.dbunit.dataset.IDataSet;
import org.dbunit.dataset.xml.FlatXmlDataSet;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = { "classpath:**/test-applicationContext.xml" })
public class RemoveAll {

	@Autowired
	private DataSource ds;

	@Test
	public void getDBasXML() throws Exception {
		DatabaseDataSourceConnection connection = new DatabaseDataSourceConnection(ds);
		// IDataSet dataSet = connection.createDataSet();
		// XmlDataSetWriter writer = new XmlDataSetWriter(new
		// FileWriter("db.xml") ,null);
		// writer.setPrettyPrint(true);
		// writer.write(dataSet);
		IDataSet fullDataSet = connection.createDataSet();
		FlatXmlDataSet.write(fullDataSet, new FileOutputStream("full.xml"));
	}
	
	@Test
	public void RemoveAllAlunosDemandas(){
		Long idInstituicaoEnsino=(long) 3;
		Long idCenario=(long) 192;
		
		//InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(idInstituicaoEnsino);
		//Cenario cenario = Cenario.find(idCenario, instituicaoEnsino);
		
		InstituicaoEnsino instituicaoEnsino = new InstituicaoEnsino();
		instituicaoEnsino =InstituicaoEnsino.find(idInstituicaoEnsino);
		
		Cenario cenario = new Cenario();
		cenario = Cenario.find(idCenario, instituicaoEnsino);
		
		AlunoDemanda.removeAllAlunosDemanda(cenario);
		
	}
	
	/*
	@Test
	public void RemoveAllEquivalencias(){
		final EquivalenciasServiceAsync service = Services.equivalencias();
		
		service.removeAll(cenarioDTO, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
			@Override
			public void onSuccess(Void result) {
				display.getGrid().updateList();
				Info.display("Removido", "Equivalencias removidas com sucesso!");
			}
		});
	}
	*/
}
