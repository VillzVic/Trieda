package com.gapso.trieda.domain;

import static org.junit.Assert.assertTrue;

import java.util.List;

import javax.sql.DataSource;

import org.dbunit.database.DatabaseDataSourceConnection;
import org.dbunit.dataset.xml.XmlDataSet;
import org.dbunit.operation.DatabaseOperation;
import org.hibernate.LazyInitializationException;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractTransactionalJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = { "/META-INF/spring/test-applicationContext.xml" })
public class GrupoSalaTest extends AbstractTransactionalJUnit4SpringContextTests {

	@Autowired
	private DataSource ds;
	private final XmlDataSet dataSet;

	public GrupoSalaTest() throws Exception {
		dataSet = new XmlDataSet(getClass().getResourceAsStream("/db.xml"));
	}

	@Before
	public void init() throws Exception {
		DatabaseOperation.CLEAN_INSERT.execute(new DatabaseDataSourceConnection(ds), dataSet);
	}

	@Test
	public void testGetSalas() {
		List<GrupoSala> findAll = GrupoSala.findAll();
		assertTrue(findAll.size()>0);
	}

	@Test(expected = LazyInitializationException.class)
	public void testLIE_1() {
		List<GrupoSala> grupos = GrupoSala.findAll();
		grupos.get(0).getDisciplinas().add(new Disciplina());
		grupos.get(0).merge();
	}

	@Test
	public void testLIE_2() {
		List<GrupoSala> grupos = GrupoSala.findAll();
		grupos.get(0).getSalas().add(Sala.find(4l));
		grupos.get(0).merge();
	}

}
