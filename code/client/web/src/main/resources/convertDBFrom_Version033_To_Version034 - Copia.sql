DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version033_To_Version034` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version033_To_Version034`()
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

    IF actual_db_version = 33 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      /* TRIEDA-1815: Alterações no Banco de Dados */
      CREATE TABLE  `trieda`.`disponibilidades` (
        `id` bigint(20) NOT NULL AUTO_INCREMENT,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
         PRIMARY KEY (`id`),
         UNIQUE KEY `dis_uniq_key` (`hor_inicio`, `hor_fim`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidades_campi` (
        `cam_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
         KEY `FK_DISP_CAM` (`cam_id`),
         CONSTRAINT `FK_DISP_CAM` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
	  CREATE TABLE  `trieda`.`disponibilidade_disciplinas` (
        `dis_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
         KEY `FK_DISP_DIS` (`dis_id`),
         CONSTRAINT `FK_DISP_DIS` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidade_professores` (
        `prf_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
         KEY `FK_DISP_PRF` (`prf_id`),
         CONSTRAINT `FK_DISP_PRF` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidade_salas` (
        `sal_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
         KEY `FK_DISP_SAL` (`sal_id`),
         CONSTRAINT `FK_DISP_SAL` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidade_unidades` (
        `uni_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
         KEY `FK_DISP_UNI` (`uni_id`),
         CONSTRAINT `FK_DISP_UNI` FOREIGN KEY (`uni_id`) REFERENCES `unidades` (`uni_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      CREATE TABLE  `trieda`.`disponibilidade_fixacoes` (
        `fix_id` bigint(20) NOT NULL,
        `hor_inicio` DATETIME NOT NULL,
        `hor_fim` DATETIME NOT NULL,
         KEY `FK_DISP_FIX` (`fix_id`),
         CONSTRAINT `FK_DISP_FIX` FOREIGN KEY (`fix_id`) REFERENCES `fixacoes` (`fix_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      
      	 INSERT IGNORE INTO disponibilidade (hor_inicio, hor_fim) select
	      min( PreQuery.hor_horario ) as FirstActionDate,
	      max( PreQuery.hor_fim ) as LastActionDate
	  	 from
	      	( select
	              hor_horario,
	              @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
	              @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
	         from (select hor_horario, sle_tempo from
		horario_disponivel_cenario_professores h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
		JOIN semana_letiva sl ON sl.sle_id = ha.sle_id group by hor_horario order by sl.sle_id, hor_horario) as t,
	              ( select @lastSeq := 0 ) sqlVars ) PreQuery
		group by
		PreQuery.ActionSeq
      
	 INSERT INTO disponibilidade_campi (cam_id, hor_inicio, hor_fim) SELECT campi, hor_horario, DATE_ADD(hor_horario, INTERVAL sle_tempo minute)
	 FROM horario_disponivel_cenario_campi h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
	 JOIN semana_letiva sl ON sl.sle_id = ha.sle_id
	 group by hor_horario, campi;
	 
	 INSERT INTO disponibilidade_professores (prf_id, hor_inicio, hor_fim) SELECT professores, hor_horario, DATE_ADD(hor_horario, INTERVAL sle_tempo minute)
	 FROM horario_disponivel_cenario_professores h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
	 JOIN semana_letiva sl ON sl.sle_id = ha.sle_id
	 group by hor_horario, professores;
	 
	 INSERT INTO disponibilidade_salas (sal_id, hor_inicio, hor_fim) SELECT salas, hor_horario, DATE_ADD(hor_horario, INTERVAL sle_tempo minute)
	 FROM horario_disponivel_cenario_salas h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
	 JOIN semana_letiva sl ON sl.sle_id = ha.sle_id
	 group by hor_horario, salas;
	 
	 INSERT INTO disponibilidade_unidades (uni_id, hor_inicio, hor_fim) SELECT unidades, hor_horario, DATE_ADD(hor_horario, INTERVAL sle_tempo minute)
	 FROM horario_disponivel_cenario_unidades h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
	 JOIN semana_letiva sl ON sl.sle_id = ha.sle_id
	 group by hor_horario, unidades;
	 
	 INSERT INTO disponibilidade_disciplinas (dis_id, hor_inicio, hor_fim) SELECT disciplinas, hor_horario, DATE_ADD(hor_horario, INTERVAL sle_tempo minute)
	 FROM horario_disponivel_cenario_disciplinas h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
	 JOIN semana_letiva sl ON sl.sle_id = ha.sle_id
	 group by hor_horario, disciplinas;
	 
	 INSERT INTO disponibilidade_fixacoes (fix_id, hor_inicio, hor_fim) SELECT fixacoes, hor_horario, DATE_ADD(hor_horario, INTERVAL sle_tempo minute)
	 FROM horario_disponivel_cenario_fixacoes h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
	 JOIN semana_letiva sl ON sl.sle_id = ha.sle_id
	 group by hor_horario, fixacoes;
	 
	 select
      min( PreQuery.hor_horario ) as FirstActionDate,
      max( PreQuery.hor_fim ) as LastActionDate,
      PreQuery.disciplinas,
      PreQuery.dia_semana
   from
      ( select
              hor_horario, disciplinas, dia_semana,
              @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
              @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
           from (select hor_horario, sle_tempo, disciplinas, dia_semana from
	 horario_disponivel_cenario_disciplinas h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
	 JOIN semana_letiva sl ON sl.sle_id = ha.sle_id group by disciplinas, hor_horario, dia_semana order by sl.sle_id, disciplinas, dia_semana, hor_horario) as t,
              ( select @lastSeq := 0 ) sqlVars ) PreQuery
group by
PreQuery.ActionSeq

select
        professores,
	      min( PreQuery.hor_horario ) as FirstActionDate,
	      max( PreQuery.hor_fim ) as LastActionDate,
        dia_semana
	  	 from
	      	( select
                professores,
	              hor_horario,
                dia_semana,
	              @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
	              @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
	         from (select professores,hor_horario, dia_semana, sle_tempo from
		horario_disponivel_cenario_professores h JOIN horario_disponivel_cenario hdc ON hdc.hdc_id = h.horarios JOIN horarios_aula ha on ha.hor_id = hdc.hor_id
		JOIN semana_letiva sl ON sl.sle_id = ha.sle_id group by professores, hor_horario, dia_semana order by dia_semana, professores, sl.sle_id, hor_horario) as t,
	              ( select @lastSeq := 0 ) sqlVars ) PreQuery
		group by
		PreQuery.ActionSeq
		
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
from (select
        disciplinas,
	      min( PreQuery.hor_horario ) as FirstActionDate,
	      max( PreQuery.hor_fim ) as LastActionDate,
        dia_semana
	  	 from
	      	( select
                disciplinas,
	              hor_horario,
                dia_semana,
	              @lastSeq := IF(hor_horario = @lastHor , @lastSeq, @lastSeq +1) as ActionSeq,
	              @lastHor := DATE_ADD(hor_horario, INTERVAL sle_tempo minute) as hor_fim
	         from (select disciplinas,hor_horario, dia_semana, sle_tempo from
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
	      LastActionDate


      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (34);
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

CALL convertDBFrom_Version033_To_Version034();