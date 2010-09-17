package com.gapso.trieda.spring;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import org.scannotation.AnnotationDB;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.orm.jpa.persistenceunit.MutablePersistenceUnitInfo;
import org.springframework.orm.jpa.persistenceunit.PersistenceUnitPostProcessor;

public class TestPersistenceUnitPostProcessor implements  PersistenceUnitPostProcessor  {
  private final Logger log = LoggerFactory.getLogger( TestPersistenceUnitPostProcessor.class );

  public void postProcessPersistenceUnitInfo(MutablePersistenceUnitInfo pui) {
    
	  
	  URL persistenceUnitRootUrl = pui.getPersistenceUnitRootUrl();

    String protocol = persistenceUnitRootUrl.getProtocol();
    String host =  persistenceUnitRootUrl.getHost();
    int port = persistenceUnitRootUrl.getPort();
    String file = persistenceUnitRootUrl.getFile();
	  if(!file.contains("test-classes")){
		  return;
	  }
    String newFile = file.substring(0, file.lastIndexOf("test-classes")) + "classes/";

    URL url = null;
    try {
      url = new URL(protocol, host, port, newFile);
    } catch (MalformedURLException e) {
      log.warn( "Malformed url: " + url, e );
      return;
    }

    AnnotationDB annotationDB = new AnnotationDB();
    try {
      annotationDB.scanArchives(url);
    } catch (IOException e) {
      log.warn( "Unexpected IOException", e );
      return;
    }

    if (annotationDB.getAnnotationIndex().containsKey("javax.persistence.Entity")) {
      for (String entity : annotationDB.getAnnotationIndex().get("javax.persistence.Entity")) {
        pui.addManagedClassName(entity);
        log.debug( "Adding: {}", entity );
      }
    }
  }

}