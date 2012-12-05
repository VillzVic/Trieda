DELIMITER $$

DROP PROCEDURE IF EXISTS `checks` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `checks`(IN campus_id INT)
BEGIN
  DECLARE total_qtd_alunos_demandas_P1 INT;
  DECLARE total_cred_alunos_demandas_P1 INT;
  DECLARE total_qtd_alunos_demandas_P1_ocupam_grade INT;
  DECLARE total_cred_alunos_demandas_P1_ocupam_grade INT;
  DECLARE total_qtd_alunos_demandas_P1_ocupam_grade_min_turmas INT;
  DECLARE total_qtd_alunos_demandas_P1_atendidos INT;
  DECLARE total_cred_alunos_demandas_P1_atendidos INT;
  DECLARE total_qtd_alunos_demandas_P1_nao_atendidos INT;
  DECLARE total_cred_alunos_demandas_P1_nao_atendidos INT;
  DECLARE total_qtd_alunos_demandas_P2_atendidos INT;
  DECLARE total_qtd_alunos_demandas_P2_atendidos_em_excesso INT;

  SET @campus_id = campus_id;
  set @min_turmas = 10;

  /* alunos demandas de prioridade 1 */
  SET total_qtd_alunos_demandas_P1 = (
  	SELECT COUNT(*) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  	WHERE
  		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  		AND
  		(ald.ald_prioridade = 1 and ofe.cam_id = @campus_id)
  );
  SET total_cred_alunos_demandas_P1 = (
  	SELECT SUM(dis.dis_cred_pratico + dis.dis_cred_teorico) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  	WHERE
  		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  		AND
  		(ald.ald_prioridade = 1 and ofe.cam_id = @campus_id)
  );
  
  /* alunos demandas de prioridade 1 do campus 2 (descontando as demandas online e de creditos zerados) */
  SET total_qtd_alunos_demandas_P1_ocupam_grade = (
	SELECT COUNT(*) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
	WHERE
		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
		AND
		(ald.ald_prioridade = 1 and ofe.cam_id = @campus_id and (dis.dis_cred_pratico + dis.dis_cred_teorico) > 0 and dis.tdi_id <> 3)
  );
  SET total_cred_alunos_demandas_P1_ocupam_grade = (
	SELECT SUM(dis.dis_cred_pratico + dis.dis_cred_teorico) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
	WHERE
		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
		AND
		(ald.ald_prioridade = 1 and ofe.cam_id = @campus_id and (dis.dis_cred_pratico + dis.dis_cred_teorico) > 0 and dis.tdi_id <> 3)
  );
  
  /* alunos demandas de prioridade 1 do campus 2 (descontando as demandas online, de creditos zerados, e acima de 10 alunos) */
  SET total_qtd_alunos_demandas_P1_ocupam_grade_min_turmas = (
  	SELECT SUM(query1.qtd_dem) FROM
  		(SELECT dem.dis_id, COUNT(ald.ald_id) as qtd_dem FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  			WHERE(
  				ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  				AND
  				(ald.ald_prioridade = 1 and ofe.cam_id = @campus_id and (dis.dis_cred_pratico + dis.dis_cred_teorico) > 0 and dis.tdi_id <> 3)
  			GROUP BY dem.dis_id
  		) as query1
  	WHERE query1.qtd_dem >= @min_turmas
  );
  
  /* alunos demandas de prioridade 1 do campus 2 ATENDIDOS (descontando as demandas online e de creditos zerados) */
  SET total_qtd_alunos_demandas_P1_atendidos = (
  	SELECT COUNT(*) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  	WHERE
  		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  		AND
  		(ald.ald_prioridade = 1 and ald.ald_atendido = b'1' and ofe.cam_id = @campus_id and (dis.dis_cred_pratico + dis.dis_cred_teorico) > 0 and dis.tdi_id <> 3)
  );
  SET total_cred_alunos_demandas_P1_atendidos = (
  	SELECT SUM(dis.dis_cred_pratico + dis.dis_cred_teorico) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  	WHERE
  		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  		AND
  		(ald.ald_prioridade = 1 and ald.ald_atendido = b'1' and ofe.cam_id = @campus_id and (dis.dis_cred_pratico + dis.dis_cred_teorico) > 0 and dis.tdi_id <> 3)
  );
  
  /* alunos demandas de prioridade 1 do campus 2 NAO ATENDIDOS (descontando as demandas online e de creditos zerados) */
  SET total_qtd_alunos_demandas_P1_nao_atendidos = (
  	SELECT COUNT(*) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  	WHERE
  		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  		AND
  		(ald.ald_prioridade = 1 and ald.ald_atendido = b'0' and ofe.cam_id = @campus_id and (dis.dis_cred_pratico + dis.dis_cred_teorico) > 0 and dis.tdi_id <> 3)
  );
  SET total_cred_alunos_demandas_P1_nao_atendidos = (
  	SELECT SUM(dis.dis_cred_pratico + dis.dis_cred_teorico) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  	WHERE
  		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  		AND
  		(ald.ald_prioridade = 1 and ald.ald_atendido = b'0' and ofe.cam_id = @campus_id and (dis.dis_cred_pratico + dis.dis_cred_teorico) > 0 and dis.tdi_id <> 3)
  );
  
  /* alunos demandas de prioridade 2 do campus 2 atendidos */
  SET total_qtd_alunos_demandas_P2_atendidos = (
  	SELECT COUNT(*) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  	WHERE
  		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  		AND
  		(ald.ald_prioridade = 2 and ofe.cam_id = @campus_id and ald_atendido = b'1')
  );
  
  /* alunos COM demandas de prioridade 2 do campus 2 ATENDIDOS que nao pertencem ao conjunto de alunos com demandas de prioridade 1 NAO ATENDIDOS */
  SET total_qtd_alunos_demandas_P2_atendidos_em_excesso = (
  	SELECT COUNT(DISTINCT aln_id) FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  	WHERE
  		(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  		AND
  		(ald.ald_prioridade = 2 and ofe.cam_id = @campus_id and ald_atendido = b'1')
  		AND
  		(ald.aln_id NOT IN (
  			SELECT DISTINCT ald.aln_id FROM alunos_demanda ald, demandas dem, ofertas ofe, disciplinas dis
  			WHERE
  				(ald.dem_id = dem.dem_id and dem.ofe_id = ofe.ofe_id and dem.dis_id = dis.dis_id)
  				AND
  				(ald.ald_prioridade = 1 and ald.ald_atendido = b'0' and ofe.cam_id = @campus_id and (dis.dis_cred_pratico + dis.dis_cred_teorico) > 0 and dis.tdi_id <> 3)
  			)
  		)
  );
	
  SELECT @campus_id AS campuId, 
  		 total_qtd_alunos_demandas_P1 AS demP1, 
  		 total_cred_alunos_demandas_P1 AS demP1Ccred,
  		 total_qtd_alunos_demandas_P1_ocupam_grade AS demP1Ppresencial,
  		 total_cred_alunos_demandas_P1_ocupam_grade AS demP1CredPresencial,
  		 total_qtd_alunos_demandas_P1_ocupam_grade_min_turmas AS demP1PresencialMinTurma,
  		 total_qtd_alunos_demandas_P1_atendidos AS demP1Atendidos,
  		 total_cred_alunos_demandas_P1_atendidos AS demP1CredAtendidos,
  		 total_qtd_alunos_demandas_P1_nao_atendidos AS demP1NaoAtendidos,
  		 total_cred_alunos_demandas_P1_nao_atendidos AS demP1CredNaoAtendidos,
  		 total_qtd_alunos_demandas_P2_atendidos AS demP2Atendidos,
  		 total_qtd_alunos_demandas_P2_atendidos_em_excesso AS demP2AtendidosExcesso;
END $$

DELIMITER;

CALL checks();