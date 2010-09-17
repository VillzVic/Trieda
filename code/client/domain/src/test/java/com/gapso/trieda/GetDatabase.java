package com.gapso.trieda;

import java.io.FileWriter;

import javax.sql.DataSource;

import org.dbunit.database.DatabaseDataSourceConnection;
import org.dbunit.database.QueryDataSet;
import org.dbunit.dataset.xml.XmlDataSetWriter;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = { "/META-INF/spring/applicationContext.xml" })
public class GetDatabase {

	@Autowired
	private DataSource ds;

	@Test
	public void getDBasXML() throws Exception {
		QueryDataSet dataSet = new QueryDataSet(new DatabaseDataSourceConnection(ds));
		dataSet.addTable("campi");
		dataSet.addTable("unidades");
		dataSet.addTable("tipos_sala");
		dataSet.addTable("salas");
		dataSet.addTable("grupos_sala");
		dataSet.addTable("deslocamentos_campi");
		dataSet.addTable("deslocamentos_unidades");
		XmlDataSetWriter writer = new XmlDataSetWriter(new FileWriter("db.xml") ,null);
		writer.setPrettyPrint(true);
		writer.write(dataSet);
	}
}
