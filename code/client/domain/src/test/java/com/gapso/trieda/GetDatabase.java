package com.gapso.trieda;

import java.io.FileOutputStream;
import java.io.FileWriter;

import javax.sql.DataSource;

import org.dbunit.database.DatabaseDataSourceConnection;
import org.dbunit.dataset.IDataSet;
import org.dbunit.dataset.xml.FlatXmlDataSet;
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
		DatabaseDataSourceConnection connection = new DatabaseDataSourceConnection(ds);
		// IDataSet dataSet = connection.createDataSet();
		// XmlDataSetWriter writer = new XmlDataSetWriter(new
		// FileWriter("db.xml") ,null);
		// writer.setPrettyPrint(true);
		// writer.write(dataSet);
		IDataSet fullDataSet = connection.createDataSet();
		FlatXmlDataSet.write(fullDataSet, new FileOutputStream("full.xml"));
	}
}
