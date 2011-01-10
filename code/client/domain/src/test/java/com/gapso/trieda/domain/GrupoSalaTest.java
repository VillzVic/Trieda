package com.gapso.trieda.domain;

import java.sql.Connection;

import javax.sql.DataSource;

import org.dbunit.database.DatabaseConnection;
import org.dbunit.database.IDatabaseConnection;
import org.dbunit.dataset.IDataSet;
import org.dbunit.dataset.xml.FlatXmlDataSetBuilder;
import org.dbunit.operation.DatabaseOperation;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.datasource.DataSourceUtils;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractTransactionalJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import static org.junit.Assert.*;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = { "/META-INF/spring/test-applicationContext.xml" })
public class GrupoSalaTest extends AbstractTransactionalJUnit4SpringContextTests 
{

	@Autowired
	private DataSource ds;

	public GrupoSalaTest() throws Exception {
	}

	@Before
	public void init() throws Exception {
		Connection con = DataSourceUtils.getConnection(ds);
		IDatabaseConnection dbUnitCon = new DatabaseConnection(con);
		FlatXmlDataSetBuilder builder = new FlatXmlDataSetBuilder();
		IDataSet dataSet = builder.build(getClass().getResourceAsStream("/full.xml"));
		try {
			DatabaseOperation.REFRESH.execute(dbUnitCon, dataSet);
		} finally {
			DataSourceUtils.releaseConnection(con, ds);
		}

		Professor p = Professor.find((long) 4);
		p.getHorarios().addAll(HorarioDisponivelCenario.findAllHorarioDisponivelCenarios());
		p.merge();
		p.detach();
		System.out.println("****************");
		
//		HorarioDisponivelCenario hdc = HorarioDisponivelCenario.findHorarioDisponivelCenario((long) 114);
//		hdc.getProfessores().add(p);
//		hdc.merge();
//		hdc.detach();
//		System.out.println("****************");
	}

	@Test
	public void testHorariosProfessor() {
		Professor p = Professor.find((long) 4);
		assertEquals(6, p.getHorarios().size());
		System.out.println("****************");

//		HorarioDisponivelCenario hdc = HorarioDisponivelCenario.findHorarioDisponivelCenario((long) 114);
//		assertEquals(1, hdc.getProfessores().size());
//		System.out.println("****************");
	}
	

	@Test
	public void testRemoverHorariosProfessor() {
		Professor p = Professor.find((long) 4);
		HorarioDisponivelCenario hdc = HorarioDisponivelCenario.findHorarioDisponivelCenario((long) 114);
		p.getHorarios().remove(hdc);
		p.merge();
		p.detach();
		p = Professor.find((long) 4);
		assertEquals(5, p.getHorarios().size());
		
	}
}
