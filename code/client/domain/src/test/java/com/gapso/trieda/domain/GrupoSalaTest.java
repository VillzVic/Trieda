package com.gapso.trieda.domain;

import java.io.InputStream;
import java.sql.SQLException;
import java.util.List;

import javax.sql.DataSource;

import org.dbunit.DBTestCase;
import org.dbunit.database.DatabaseDataSourceConnection;
import org.dbunit.dataset.IDataSet;
import org.dbunit.dataset.xml.XmlDataSet;
import org.dbunit.operation.DatabaseOperation;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = { "/META-INF/spring/test-applicationContext.xml" })
public class GrupoSalaTest extends DBTestCase {

	@Autowired
	private DataSource ds;
	
	@Test
	public void testGetSalas() throws Exception {
		
		DatabaseDataSourceConnection conn = new DatabaseDataSourceConnection(ds);
		
		DatabaseOperation.INSERT.execute(conn, getDataSet());
		
		List<GrupoSala> findAll = GrupoSala.findAll();
		for (GrupoSala grupoSala : findAll) {
			System.out.println(grupoSala.getCodigo() + " " + grupoSala.getNome());
		}
		
//		GrupoSala gs = new GrupoSala();
//		gs.setNome("GRUPO_SALA_TESTE");
//		gs.getSalas().add(createSala("SALA1"));
//		gs.getSalas().add(createSala("SALA2"));
//		gs.getSalas().add(createSala("SALA3"));
//		gs.getSalas().add(createSala("SALA4"));
//		gs.persist();
//		gs.flush();
	}

	private Sala createSala(String codigo) {
		Sala sala = new Sala();
		sala.setCodigo(codigo);
		sala.persist();
		return sala;
	}

	@Override
	protected IDataSet getDataSet() throws Exception {
		InputStream flatFile = getClass().getResourceAsStream("/db.xml");
		return new XmlDataSet(flatFile);
	}
}
