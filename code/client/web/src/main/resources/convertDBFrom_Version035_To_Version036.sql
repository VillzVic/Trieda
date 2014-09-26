DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version035_To_Version036` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version035_To_Version036`()
BEGIN
  DECLARE status_code INT;
  DECLARE count_table_db_version INT;
  DECLARE actual_db_version INT;
  DECLARE msg_status VARCHAR(500);

  /**
   * exit handler for anything that goes wrong during execution. This will
   * ensure the any subsequent DML statements are rolled back
   */
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    SET status_code = -1;
    ROLLBACK;
    SELECT status_code AS status_code, msg_status AS msg_status;
  END;

  SET status_code = 0;
  SET msg_status = 'Inicio da Transacao';

  /* start the transaction - so that anything that follows will be atomic */
  START TRANSACTION;

    /* verifica em que versao a base de dados se encontra */
    SET msg_status = 'Inicio verificacao qual a versao da base de dados';
    SET @schema = 'trieda';
    SET @table_name = 'db_version';
    SELECT COUNT(*) INTO count_table_db_version FROM information_schema.tables WHERE table_schema = @schema AND table_name LIKE @table_name;
    IF count_table_db_version < 1 THEN
      CREATE TABLE `trieda`.`db_version` (
        `db_version` INTEGER UNSIGNED NOT NULL,
        PRIMARY KEY (`db_version`)
      ) ENGINE = InnoDB;
      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (0);
    END IF;
    SET actual_db_version = (SELECT MAX(`db_version`) FROM `trieda`.`db_version`);
    SET msg_status = 'Versao da base de dados identificada';

    IF actual_db_version = 35 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      /* TRIEDA: Criando superuser */
	  CREATE TABLE  `trieda`.`disponibilidade_disciplinas` (
	    `dpd_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `dis_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
        `segunda` BIT(1) DEFAULT TRUE,
        `terca` BIT(1) DEFAULT TRUE,
        `quarta` BIT(1) DEFAULT TRUE,
        `quinta` BIT(1) DEFAULT TRUE,
        `sexta` BIT(1) DEFAULT TRUE,
        `sabado` BIT(1) DEFAULT TRUE,
        `domingo` BIT(1) DEFAULT TRUE,
         PRIMARY KEY (`dpd_id`),
         KEY `FK_DISP_DIS` (`dis_id`),
         CONSTRAINT `FK_DISP_DIS` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidade_professores` (
	    `dpp_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `prf_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
        `segunda` BIT(1) DEFAULT TRUE,
        `terca` BIT(1) DEFAULT TRUE,
        `quarta` BIT(1) DEFAULT TRUE,
        `quinta` BIT(1) DEFAULT TRUE,
        `sexta` BIT(1) DEFAULT TRUE,
        `sabado` BIT(1) DEFAULT TRUE,
        `domingo` BIT(1) DEFAULT TRUE,
         PRIMARY KEY (`dpp_id`),
         KEY `FK_DISP_PROF` (`prf_id`),
         CONSTRAINT `FK_DISP_PROF` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidade_campi` (
	    `dpc_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `cam_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
        `segunda` BIT(1) DEFAULT TRUE,
        `terca` BIT(1) DEFAULT TRUE,
        `quarta` BIT(1) DEFAULT TRUE,
        `quinta` BIT(1) DEFAULT TRUE,
        `sexta` BIT(1) DEFAULT TRUE,
        `sabado` BIT(1) DEFAULT TRUE,
        `domingo` BIT(1) DEFAULT TRUE,
         PRIMARY KEY (`dpc_id`),
         KEY `FK_DISP_CAM` (`cam_id`),
         CONSTRAINT `FK_DISP_CAM` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidade_unidades` (
	    `dpu_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `uni_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
        `segunda` BIT(1) DEFAULT TRUE,
        `terca` BIT(1) DEFAULT TRUE,
        `quarta` BIT(1) DEFAULT TRUE,
        `quinta` BIT(1) DEFAULT TRUE,
        `sexta` BIT(1) DEFAULT TRUE,
        `sabado` BIT(1) DEFAULT TRUE,
        `domingo` BIT(1) DEFAULT TRUE,
         PRIMARY KEY (`dpu_id`),
         KEY `FK_DISP_UNI` (`uni_id`),
         CONSTRAINT `FK_DISP_UNI` FOREIGN KEY (`uni_id`) REFERENCES `unidades` (`uni_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidade_salas` (
	    `dps_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `sal_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
        `segunda` BIT(1) DEFAULT TRUE,
        `terca` BIT(1) DEFAULT TRUE,
        `quarta` BIT(1) DEFAULT TRUE,
        `quinta` BIT(1) DEFAULT TRUE,
        `sexta` BIT(1) DEFAULT TRUE,
        `sabado` BIT(1) DEFAULT TRUE,
        `domingo` BIT(1) DEFAULT TRUE,
         PRIMARY KEY (`dps_id`),
         KEY `FK_DISP_SAL` (`sal_id`),
         CONSTRAINT `FK_DISP_SAL` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      INSERT INTO disponibilidade_disciplinas (dis_id, hor_inicio, hor_fim, segunda, terca, quarta, quinta, sexta, sabado, domingo)
      select
         disciplinas,
	     FirstActionDate,
	     LastActionDate,
         sum(case when dia_semana=0 then 1 else 0 end) as segunda,
         sum(case when dia_semana=1 then 1 else 0 end) as terca,
         sum(case when dia_semana=2 then 1 else 0 end) as quarta,
         sum(case when dia_semana=3 then 1 else 0 end) as quinta,
         sum(case when dia_semana=4 then 1 else 0 end) as sexta,
         sum(case when dia_semana=4 then 1 else 0 end) as sabado,
         sum(case when dia_semana=5 then 1 else 0 end) as domingo
      from (
         select
            disciplinas,
	        min( PreQuery.hor_horario ) as FirstActionDate,
	        max( PreQuery.hor_fim ) as LastActionDate,
            dia_semana
	  	 from ( 
	        select
               disciplinas,
	           hor_horario,
               dia_semana,
	           @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
	           @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
	        from (
	           select disciplinas,hor_horario, dia_semana, sle_tempo
	           from
		          horario_disponivel_cenario_disciplinas h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
		          JOIN semana_letiva sl ON sl.sle_id = ha.sle_id group by disciplinas, hor_horario, dia_semana order by disciplinas, dia_semana, sl.sle_id, hor_horario) as t,
	              ( select @lastSeq := 0 ) sqlVars ) PreQuery
		    group by
		    PreQuery.ActionSeq
        order by
           disciplinas, FirstActionDate, LastActionDate) as a
     group by
        disciplinas,
	    FirstActionDate,
	    LastActionDate;
	    
	 INSERT INTO disponibilidade_professores (prf_id, hor_inicio, hor_fim, segunda, terca, quarta, quinta, sexta, sabado, domingo)
      select
         professores,
	     FirstActionDate,
	     LastActionDate,
         sum(case when dia_semana=0 then 1 else 0 end) as segunda,
         sum(case when dia_semana=1 then 1 else 0 end) as terca,
         sum(case when dia_semana=2 then 1 else 0 end) as quarta,
         sum(case when dia_semana=3 then 1 else 0 end) as quinta,
         sum(case when dia_semana=4 then 1 else 0 end) as sexta,
         sum(case when dia_semana=4 then 1 else 0 end) as sabado,
         sum(case when dia_semana=5 then 1 else 0 end) as domingo
      from (
         select
            professores,
	        min( PreQuery.hor_horario ) as FirstActionDate,
	        max( PreQuery.hor_fim ) as LastActionDate,
            dia_semana
	  	 from ( 
	        select
               professores,
	           hor_horario,
               dia_semana,
	           @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
	           @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
	        from (
	           select professores,hor_horario, dia_semana, sle_tempo
	           from
		          horario_disponivel_cenario_professores h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
		          JOIN semana_letiva sl ON sl.sle_id = ha.sle_id group by professores, hor_horario, dia_semana order by professores, dia_semana, sl.sle_id, hor_horario) as t,
	              ( select @lastSeq := 0 ) sqlVars ) PreQuery
		    group by
		    PreQuery.ActionSeq
        order by
           professores, FirstActionDate, LastActionDate) as a
     group by
        professores,
	    FirstActionDate,
	    LastActionDate;

	 INSERT INTO disponibilidade_campi (cam_id, hor_inicio, hor_fim, segunda, terca, quarta, quinta, sexta, sabado, domingo)
      select
         campi,
	     FirstActionDate,
	     LastActionDate,
         sum(case when dia_semana=0 then 1 else 0 end) as segunda,
         sum(case when dia_semana=1 then 1 else 0 end) as terca,
         sum(case when dia_semana=2 then 1 else 0 end) as quarta,
         sum(case when dia_semana=3 then 1 else 0 end) as quinta,
         sum(case when dia_semana=4 then 1 else 0 end) as sexta,
         sum(case when dia_semana=4 then 1 else 0 end) as sabado,
         sum(case when dia_semana=5 then 1 else 0 end) as domingo
      from (
         select
            campi,
	        min( PreQuery.hor_horario ) as FirstActionDate,
	        max( PreQuery.hor_fim ) as LastActionDate,
            dia_semana
	  	 from ( 
	        select
               campi,
	           hor_horario,
               dia_semana,
	           @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
	           @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
	        from (
	           select campi,hor_horario, dia_semana, sle_tempo
	           from
		          horario_disponivel_cenario_campi h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
		          JOIN semana_letiva sl ON sl.sle_id = ha.sle_id group by campi, hor_horario, dia_semana order by campi, dia_semana, sl.sle_id, hor_horario) as t,
	              ( select @lastSeq := 0 ) sqlVars ) PreQuery
		    group by
		    PreQuery.ActionSeq
        order by
           campi, FirstActionDate, LastActionDate) as a
     group by
        campi,
	    FirstActionDate,
	    LastActionDate;
	    
	 INSERT INTO disponibilidade_unidades (uni_id, hor_inicio, hor_fim, segunda, terca, quarta, quinta, sexta, sabado, domingo)
      select
         unidades,
	     FirstActionDate,
	     LastActionDate,
         sum(case when dia_semana=0 then 1 else 0 end) as segunda,
         sum(case when dia_semana=1 then 1 else 0 end) as terca,
         sum(case when dia_semana=2 then 1 else 0 end) as quarta,
         sum(case when dia_semana=3 then 1 else 0 end) as quinta,
         sum(case when dia_semana=4 then 1 else 0 end) as sexta,
         sum(case when dia_semana=4 then 1 else 0 end) as sabado,
         sum(case when dia_semana=5 then 1 else 0 end) as domingo
      from (
         select
            unidades,
	        min( PreQuery.hor_horario ) as FirstActionDate,
	        max( PreQuery.hor_fim ) as LastActionDate,
            dia_semana
	  	 from ( 
	        select
               unidades,
	           hor_horario,
               dia_semana,
	           @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
	           @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
	        from (
	           select unidades,hor_horario, dia_semana, sle_tempo
	           from
		          horario_disponivel_cenario_unidades h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
		          JOIN semana_letiva sl ON sl.sle_id = ha.sle_id group by unidades, hor_horario, dia_semana order by unidades, dia_semana, sl.sle_id, hor_horario) as t,
	              ( select @lastSeq := 0 ) sqlVars ) PreQuery
		    group by
		    PreQuery.ActionSeq
        order by
           unidades, FirstActionDate, LastActionDate) as a
     group by
        unidades,
	    FirstActionDate,
	    LastActionDate;
	    
	 INSERT INTO disponibilidade_salas (sal_id, hor_inicio, hor_fim, segunda, terca, quarta, quinta, sexta, sabado, domingo)
      select
         salas,
	     FirstActionDate,
	     LastActionDate,
         sum(case when dia_semana=0 then 1 else 0 end) as segunda,
         sum(case when dia_semana=1 then 1 else 0 end) as terca,
         sum(case when dia_semana=2 then 1 else 0 end) as quarta,
         sum(case when dia_semana=3 then 1 else 0 end) as quinta,
         sum(case when dia_semana=4 then 1 else 0 end) as sexta,
         sum(case when dia_semana=4 then 1 else 0 end) as sabado,
         sum(case when dia_semana=5 then 1 else 0 end) as domingo
      from (
         select
            salas,
	        min( PreQuery.hor_horario ) as FirstActionDate,
	        max( PreQuery.hor_fim ) as LastActionDate,
            dia_semana
	  	 from ( 
	        select
               salas,
	           hor_horario,
               dia_semana,
	           @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
	           @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
	        from (
	           select salas,hor_horario, dia_semana, sle_tempo
	           from
		          horario_disponivel_cenario_salas h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
		          JOIN semana_letiva sl ON sl.sle_id = ha.sle_id group by salas, hor_horario, dia_semana order by salas, salas, sl.sle_id, hor_horario) as t,
	              ( select @lastSeq := 0 ) sqlVars ) PreQuery
		    group by
		    PreQuery.ActionSeq
        order by
           salas, FirstActionDate, LastActionDate) as a
     group by
        salas,
	    FirstActionDate,
	    LastActionDate;

      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (36);
      SET msg_status = 'Base de dados compativel com script de conversao, conversao realizada';
    ELSE
      SET msg_status = 'Base de dados nao compativel com script de conversao, conversao nao efetuada';
    END IF;
    
  /* if status_code of 0, then commit, else roll back */
  IF (status_code = 0) THEN
    COMMIT;
  ELSE
    ROLLBACK;
  END IF;
    
  SELECT status_code AS status_code, msg_status AS msg_status;
END $$

DELIMITER;

CALL convertDBFrom_Version035_To_Version036();