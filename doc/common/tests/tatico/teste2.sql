-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.1.32-community


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema trieda
--

CREATE DATABASE IF NOT EXISTS trieda;
USE trieda;

--
-- Definition of table `areas_titulacao`
--

DROP TABLE IF EXISTS `areas_titulacao`;
CREATE TABLE `areas_titulacao` (
  `ati_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `ati_codigo` varchar(50) NOT NULL,
  `ati_descricao` varchar(255) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`ati_id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `areas_titulacao`
--

/*!40000 ALTER TABLE `areas_titulacao` DISABLE KEYS */;
INSERT INTO `areas_titulacao` (`ati_id`,`ati_codigo`,`ati_descricao`,`version`) VALUES 
 (7,'Área de Titulação 1','Área de Titulação 1',9),
 (8,'Área de Titulação 2','Área de Titulação 2',9),
 (9,'Área de Titulação 3','Área de Titulação 3',1),
 (10,'Área de Titulação 4','Área de Titulação 4',1);
/*!40000 ALTER TABLE `areas_titulacao` ENABLE KEYS */;


--
-- Definition of table `areas_titulacao_cursos`
--

DROP TABLE IF EXISTS `areas_titulacao_cursos`;
CREATE TABLE `areas_titulacao_cursos` (
  `areas_titulacao` bigint(20) NOT NULL,
  `cursos` bigint(20) NOT NULL,
  PRIMARY KEY (`areas_titulacao`,`cursos`),
  KEY `FK38D2E30938FF8237` (`cursos`),
  KEY `FK38D2E309B4354A` (`areas_titulacao`),
  CONSTRAINT `FK38D2E30938FF8237` FOREIGN KEY (`cursos`) REFERENCES `cursos` (`cur_id`),
  CONSTRAINT `FK38D2E309B4354A` FOREIGN KEY (`areas_titulacao`) REFERENCES `areas_titulacao` (`ati_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `areas_titulacao_cursos`
--

/*!40000 ALTER TABLE `areas_titulacao_cursos` DISABLE KEYS */;
INSERT INTO `areas_titulacao_cursos` (`areas_titulacao`,`cursos`) VALUES 
 (7,7),
 (8,7),
 (9,7),
 (10,7);
/*!40000 ALTER TABLE `areas_titulacao_cursos` ENABLE KEYS */;


--
-- Definition of table `atendimento_operacional`
--

DROP TABLE IF EXISTS `atendimento_operacional`;
CREATE TABLE `atendimento_operacional` (
  `atp_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `atp_creditoteotico` bit(1) DEFAULT NULL,
  `atp_quantidade` int(11) NOT NULL,
  `turma` varchar(255) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `hdc_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) NOT NULL,
  `dis_id` bigint(20) NOT NULL,
  `ofe_id` bigint(20) NOT NULL,
  `prf_id` bigint(20) NOT NULL,
  `prv_id` bigint(20) NOT NULL,
  `sal_id` bigint(20) NOT NULL,
  PRIMARY KEY (`atp_id`),
  KEY `FK3C68D5467EDB695` (`cen_id`),
  KEY `FK3C68D54AD343A9B` (`sal_id`),
  KEY `FK3C68D54AE2A797B` (`hdc_id`),
  KEY `FK3C68D548365A75E` (`prv_id`),
  KEY `FK3C68D5427898F49` (`prf_id`),
  KEY `FK3C68D54EADC7214` (`dis_id`),
  KEY `FK3C68D547A9BEDF9` (`ofe_id`),
  CONSTRAINT `FK3C68D5427898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`),
  CONSTRAINT `FK3C68D5467EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK3C68D547A9BEDF9` FOREIGN KEY (`ofe_id`) REFERENCES `ofertas` (`cac_id`),
  CONSTRAINT `FK3C68D548365A75E` FOREIGN KEY (`prv_id`) REFERENCES `professores_virtuais` (`prf_id`),
  CONSTRAINT `FK3C68D54AD343A9B` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`),
  CONSTRAINT `FK3C68D54AE2A797B` FOREIGN KEY (`hdc_id`) REFERENCES `horario_disponivel_cenario` (`hdc_id`),
  CONSTRAINT `FK3C68D54EADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `atendimento_operacional`
--

/*!40000 ALTER TABLE `atendimento_operacional` DISABLE KEYS */;
/*!40000 ALTER TABLE `atendimento_operacional` ENABLE KEYS */;


--
-- Definition of table `atendimento_tatico`
--

DROP TABLE IF EXISTS `atendimento_tatico`;
CREATE TABLE `atendimento_tatico` (
  `att_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `att_cred_pratico` int(11) NOT NULL,
  `att_cred_teorico` int(11) NOT NULL,
  `att_quantidade` int(11) NOT NULL,
  `semana` int(11) NOT NULL,
  `turma` varchar(255) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `dis_id` bigint(20) NOT NULL,
  `ofe_id` bigint(20) NOT NULL,
  `sal_id` bigint(20) NOT NULL,
  PRIMARY KEY (`att_id`),
  KEY `FK54748EDD67EDB695` (`cen_id`),
  KEY `FK54748EDDAD343A9B` (`sal_id`),
  KEY `FK54748EDDEADC7214` (`dis_id`),
  KEY `FK54748EDD7A9BEDF9` (`ofe_id`),
  CONSTRAINT `FK54748EDD67EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK54748EDD7A9BEDF9` FOREIGN KEY (`ofe_id`) REFERENCES `ofertas` (`cac_id`),
  CONSTRAINT `FK54748EDDAD343A9B` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`),
  CONSTRAINT `FK54748EDDEADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `atendimento_tatico`
--

/*!40000 ALTER TABLE `atendimento_tatico` DISABLE KEYS */;
INSERT INTO `atendimento_tatico` (`att_id`,`att_cred_pratico`,`att_cred_teorico`,`att_quantidade`,`semana`,`turma`,`version`,`cen_id`,`dis_id`,`ofe_id`,`sal_id`) VALUES 
 (16,0,2,40,0,'TURMA0',0,1,46,10,18),
 (17,0,2,40,0,'TURMA0',0,1,39,10,18),
 (18,0,2,40,3,'TURMA0',0,1,40,10,18),
 (19,0,2,40,3,'TURMA0',0,1,42,10,18),
 (20,0,2,40,1,'TURMA0',0,1,44,10,18),
 (21,0,2,40,1,'TURMA0',0,1,38,10,18),
 (22,0,2,40,2,'TURMA0',0,1,45,10,18),
 (23,0,2,40,2,'TURMA0',0,1,41,10,18),
 (24,0,2,40,4,'TURMA0',0,1,43,10,18),
 (25,0,2,40,4,'TURMA0',0,1,37,10,18);
/*!40000 ALTER TABLE `atendimento_tatico` ENABLE KEYS */;


--
-- Definition of table `authorities`
--

DROP TABLE IF EXISTS `authorities`;
CREATE TABLE `authorities` (
  `username` varchar(50) NOT NULL,
  `authority` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `authorities`
--

/*!40000 ALTER TABLE `authorities` DISABLE KEYS */;
INSERT INTO `authorities` (`username`,`authority`) VALUES 
 ('aaaaa','ROLE_USER'),
 ('admin','ROLE_SUPERVISOR'),
 ('bbbbb','ROLE_USER'),
 ('prof','ROLE_USER'),
 ('user','ROLE_USER');
/*!40000 ALTER TABLE `authorities` ENABLE KEYS */;


--
-- Definition of table `campi`
--

DROP TABLE IF EXISTS `campi`;
CREATE TABLE `campi` (
  `cam_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `cam_bairro` varchar(25) DEFAULT NULL,
  `cam_codigo` varchar(20) NOT NULL,
  `cam_estado` int(11) DEFAULT NULL,
  `cam_municipio` varchar(25) DEFAULT NULL,
  `cam_nome` varchar(50) NOT NULL,
  `cam_publicar` bit(1) DEFAULT NULL,
  `cam_valor_credito` double DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cam_id`),
  UNIQUE KEY `cam_codigo` (`cam_codigo`,`cen_id`),
  KEY `FK5A0D60867EDB695` (`cen_id`),
  CONSTRAINT `FK5A0D60867EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `campi`
--

/*!40000 ALTER TABLE `campi` DISABLE KEYS */;
INSERT INTO `campi` (`cam_id`,`cam_bairro`,`cam_codigo`,`cam_estado`,`cam_municipio`,`cam_nome`,`cam_publicar`,`cam_valor_credito`,`version`,`cen_id`) VALUES 
 (8,'Bairro1','1',18,'Municipio1','Campus1',0x00,55,0,1);
/*!40000 ALTER TABLE `campi` ENABLE KEYS */;


--
-- Definition of table `cenarios`
--

DROP TABLE IF EXISTS `cenarios`;
CREATE TABLE `cenarios` (
  `cen_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `cen_ano` int(11) DEFAULT NULL,
  `cen_comentario` varchar(255) DEFAULT NULL,
  `cen_dt_atualizacao` datetime DEFAULT NULL,
  `cen_dt_criacao` datetime DEFAULT NULL,
  `cen_masterdata` bit(1) NOT NULL,
  `cen_nome` varchar(50) NOT NULL,
  `cen_oficial` bit(1) DEFAULT NULL,
  `cen_semestre` int(11) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `usu_atualizacao_id` varchar(50) DEFAULT NULL,
  `usu_criacao_id` varchar(50) DEFAULT NULL,
  `sle_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`cen_id`),
  KEY `FKCC2B217081EC78EA` (`usu_criacao_id`),
  KEY `FKCC2B2170BE181A06` (`usu_atualizacao_id`),
  KEY `FKCC2B21705486001C` (`sle_id`),
  CONSTRAINT `FKCC2B21705486001C` FOREIGN KEY (`sle_id`) REFERENCES `semana_letiva` (`sle_id`),
  CONSTRAINT `FKCC2B217081EC78EA` FOREIGN KEY (`usu_criacao_id`) REFERENCES `users` (`username`),
  CONSTRAINT `FKCC2B2170BE181A06` FOREIGN KEY (`usu_atualizacao_id`) REFERENCES `users` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cenarios`
--

/*!40000 ALTER TABLE `cenarios` DISABLE KEYS */;
INSERT INTO `cenarios` (`cen_id`,`cen_ano`,`cen_comentario`,`cen_dt_atualizacao`,`cen_dt_criacao`,`cen_masterdata`,`cen_nome`,`cen_oficial`,`cen_semestre`,`version`,`usu_atualizacao_id`,`usu_criacao_id`,`sle_id`) VALUES 
 (1,1,'MASTER DATA',NULL,NULL,0x01,'MASTER DATA',0x00,1,0,NULL,NULL,NULL);
/*!40000 ALTER TABLE `cenarios` ENABLE KEYS */;


--
-- Definition of table `curriculos`
--

DROP TABLE IF EXISTS `curriculos`;
CREATE TABLE `curriculos` (
  `crc_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `crc_cod` varchar(20) NOT NULL,
  `crc_descricao` varchar(255) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `cur_id` bigint(20) NOT NULL,
  PRIMARY KEY (`crc_id`),
  UNIQUE KEY `crc_cod` (`crc_cod`,`cur_id`),
  KEY `FKC5509B8767EDB695` (`cen_id`),
  KEY `FKC5509B8738FF365A` (`cur_id`),
  CONSTRAINT `FKC5509B8738FF365A` FOREIGN KEY (`cur_id`) REFERENCES `cursos` (`cur_id`),
  CONSTRAINT `FKC5509B8767EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curriculos`
--

/*!40000 ALTER TABLE `curriculos` DISABLE KEYS */;
INSERT INTO `curriculos` (`crc_id`,`crc_cod`,`crc_descricao`,`version`,`cen_id`,`cur_id`) VALUES 
 (11,'1','DescCurso1',1,1,7);
/*!40000 ALTER TABLE `curriculos` ENABLE KEYS */;


--
-- Definition of table `curriculos_disciplinas`
--

DROP TABLE IF EXISTS `curriculos_disciplinas`;
CREATE TABLE `curriculos_disciplinas` (
  `cdi_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `cdi_periodo` int(11) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `crc_id` bigint(20) NOT NULL,
  `dis_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cdi_id`),
  KEY `FKDE3CB9CFEADC7214` (`dis_id`),
  KEY `FKDE3CB9CF74E10E16` (`crc_id`),
  CONSTRAINT `FKDE3CB9CF74E10E16` FOREIGN KEY (`crc_id`) REFERENCES `curriculos` (`crc_id`),
  CONSTRAINT `FKDE3CB9CFEADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curriculos_disciplinas`
--

/*!40000 ALTER TABLE `curriculos_disciplinas` DISABLE KEYS */;
INSERT INTO `curriculos_disciplinas` (`cdi_id`,`cdi_periodo`,`version`,`crc_id`,`dis_id`) VALUES 
 (1,1,3,11,37),
 (2,1,3,11,38),
 (3,1,3,11,39),
 (4,1,3,11,40),
 (5,1,3,11,41),
 (6,2,1,11,42),
 (7,2,1,11,43),
 (8,2,1,11,44),
 (9,2,1,11,45),
 (10,2,1,11,46);
/*!40000 ALTER TABLE `curriculos_disciplinas` ENABLE KEYS */;


--
-- Definition of table `curriculos_disciplinas_salas`
--

DROP TABLE IF EXISTS `curriculos_disciplinas_salas`;
CREATE TABLE `curriculos_disciplinas_salas` (
  `curriculo_disciplinas` bigint(20) NOT NULL,
  `salas` bigint(20) NOT NULL,
  PRIMARY KEY (`curriculo_disciplinas`,`salas`),
  KEY `FK10D68E20E9EFC20F` (`salas`),
  KEY `FK10D68E20643CDB70` (`curriculo_disciplinas`),
  CONSTRAINT `FK10D68E20643CDB70` FOREIGN KEY (`curriculo_disciplinas`) REFERENCES `curriculos_disciplinas` (`cdi_id`),
  CONSTRAINT `FK10D68E20E9EFC20F` FOREIGN KEY (`salas`) REFERENCES `salas` (`sal_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curriculos_disciplinas_salas`
--

/*!40000 ALTER TABLE `curriculos_disciplinas_salas` DISABLE KEYS */;
INSERT INTO `curriculos_disciplinas_salas` (`curriculo_disciplinas`,`salas`) VALUES 
 (1,18),
 (2,18),
 (3,18),
 (4,18),
 (5,18),
 (6,18),
 (7,18),
 (8,18),
 (9,18),
 (10,18);
/*!40000 ALTER TABLE `curriculos_disciplinas_salas` ENABLE KEYS */;


--
-- Definition of table `curso_descompartilha`
--

DROP TABLE IF EXISTS `curso_descompartilha`;
CREATE TABLE `curso_descompartilha` (
  `cde_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` int(11) DEFAULT NULL,
  `cur1_id` bigint(20) NOT NULL,
  `cur2_id` bigint(20) NOT NULL,
  `par_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cde_id`),
  KEY `FK2FD03F01CC77FD68` (`cur2_id`),
  KEY `FK2FD03F01D74052F6` (`par_id`),
  KEY `FK2FD03F01CC778909` (`cur1_id`),
  CONSTRAINT `FK2FD03F01CC778909` FOREIGN KEY (`cur1_id`) REFERENCES `cursos` (`cur_id`),
  CONSTRAINT `FK2FD03F01CC77FD68` FOREIGN KEY (`cur2_id`) REFERENCES `cursos` (`cur_id`),
  CONSTRAINT `FK2FD03F01D74052F6` FOREIGN KEY (`par_id`) REFERENCES `parametros` (`par_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curso_descompartilha`
--

/*!40000 ALTER TABLE `curso_descompartilha` DISABLE KEYS */;
/*!40000 ALTER TABLE `curso_descompartilha` ENABLE KEYS */;


--
-- Definition of table `cursos`
--

DROP TABLE IF EXISTS `cursos`;
CREATE TABLE `cursos` (
  `cur_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `cur_adm_mais_de_uma_disc` bit(1) DEFAULT NULL,
  `cur_codigo` varchar(50) NOT NULL,
  `cur_max_disc_prof` int(11) NOT NULL,
  `cur_tem_int` int(11) NOT NULL,
  `cur_tem_intparc` int(11) NOT NULL,
  `cur_nome` varchar(50) NOT NULL,
  `cur_min_doutores` int(11) NOT NULL,
  `cur_min_mestres` int(11) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `tcu_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cur_id`),
  UNIQUE KEY `cen_id` (`cen_id`,`cur_codigo`),
  KEY `FKAF96135767EDB695` (`cen_id`),
  KEY `FKAF9613571A1B2A20` (`tcu_id`),
  CONSTRAINT `FKAF9613571A1B2A20` FOREIGN KEY (`tcu_id`) REFERENCES `tipos_curso` (`tcu_id`),
  CONSTRAINT `FKAF96135767EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cursos`
--

/*!40000 ALTER TABLE `cursos` DISABLE KEYS */;
INSERT INTO `cursos` (`cur_id`,`cur_adm_mais_de_uma_disc`,`cur_codigo`,`cur_max_disc_prof`,`cur_tem_int`,`cur_tem_intparc`,`cur_nome`,`cur_min_doutores`,`cur_min_mestres`,`version`,`cen_id`,`tcu_id`) VALUES 
 (7,0x00,'1',1,0,0,'Curso1',0,0,0,1,4);
/*!40000 ALTER TABLE `cursos` ENABLE KEYS */;


--
-- Definition of table `demandas`
--

DROP TABLE IF EXISTS `demandas`;
CREATE TABLE `demandas` (
  `dem_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `dem_quantidade` int(11) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `dis_id` bigint(20) NOT NULL,
  `ofe_id` bigint(20) NOT NULL,
  PRIMARY KEY (`dem_id`),
  KEY `FK32558FBDEADC7214` (`dis_id`),
  KEY `FK32558FBD7A9BEDF9` (`ofe_id`),
  CONSTRAINT `FK32558FBD7A9BEDF9` FOREIGN KEY (`ofe_id`) REFERENCES `ofertas` (`cac_id`),
  CONSTRAINT `FK32558FBDEADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `demandas`
--

/*!40000 ALTER TABLE `demandas` DISABLE KEYS */;
INSERT INTO `demandas` (`dem_id`,`dem_quantidade`,`version`,`dis_id`,`ofe_id`) VALUES 
 (1,40,9,37,10),
 (2,40,3,38,10),
 (3,40,2,39,10),
 (4,40,2,40,10),
 (5,40,2,41,10),
 (6,40,0,42,10),
 (7,40,0,43,10),
 (8,40,0,44,10),
 (9,40,0,45,10),
 (10,40,0,46,10);
/*!40000 ALTER TABLE `demandas` ENABLE KEYS */;


--
-- Definition of table `deslocamentos_campi`
--

DROP TABLE IF EXISTS `deslocamentos_campi`;
CREATE TABLE `deslocamentos_campi` (
  `dec_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `dec_custo` double DEFAULT NULL,
  `dec_tempo` int(11) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `cam_dest_id` bigint(20) NOT NULL,
  `cam_orig_id` bigint(20) NOT NULL,
  PRIMARY KEY (`dec_id`),
  KEY `FK99BD5EFED1738683` (`cam_dest_id`),
  KEY `FK99BD5EFE2CF43E84` (`cam_orig_id`),
  CONSTRAINT `FK99BD5EFE2CF43E84` FOREIGN KEY (`cam_orig_id`) REFERENCES `campi` (`cam_id`),
  CONSTRAINT `FK99BD5EFED1738683` FOREIGN KEY (`cam_dest_id`) REFERENCES `campi` (`cam_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `deslocamentos_campi`
--

/*!40000 ALTER TABLE `deslocamentos_campi` DISABLE KEYS */;
/*!40000 ALTER TABLE `deslocamentos_campi` ENABLE KEYS */;


--
-- Definition of table `deslocamentos_unidades`
--

DROP TABLE IF EXISTS `deslocamentos_unidades`;
CREATE TABLE `deslocamentos_unidades` (
  `deu_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `dec_custo` double DEFAULT NULL,
  `dec_tempo` int(11) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `uni_dest_id` bigint(20) NOT NULL,
  `uni_orig_id` bigint(20) NOT NULL,
  PRIMARY KEY (`deu_id`),
  KEY `FKD258E6CF3E0A7B7B` (`uni_dest_id`),
  KEY `FKD258E6CF998B337C` (`uni_orig_id`),
  CONSTRAINT `FKD258E6CF3E0A7B7B` FOREIGN KEY (`uni_dest_id`) REFERENCES `unidades` (`uni_id`),
  CONSTRAINT `FKD258E6CF998B337C` FOREIGN KEY (`uni_orig_id`) REFERENCES `unidades` (`uni_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `deslocamentos_unidades`
--

/*!40000 ALTER TABLE `deslocamentos_unidades` DISABLE KEYS */;
/*!40000 ALTER TABLE `deslocamentos_unidades` ENABLE KEYS */;


--
-- Definition of table `disciplinas`
--

DROP TABLE IF EXISTS `disciplinas`;
CREATE TABLE `disciplinas` (
  `dis_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `dis_codigo` varchar(20) NOT NULL,
  `dis_cred_pratico` int(11) NOT NULL,
  `dis_cred_teorico` int(11) NOT NULL,
  `dificuldade` int(11) NOT NULL,
  `dis_laboratorio` bit(1) DEFAULT NULL,
  `dis_max_alun_pratico` int(11) NOT NULL,
  `dis_max_alun_teorico` int(11) NOT NULL,
  `dis_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `dcr_id` bigint(20) DEFAULT NULL,
  `tdi_id` bigint(20) NOT NULL,
  PRIMARY KEY (`dis_id`),
  UNIQUE KEY `dis_codigo` (`dis_codigo`,`cen_id`),
  KEY `FK99F7D28767EDB695` (`cen_id`),
  KEY `FK99F7D287264F3B1D` (`tdi_id`),
  KEY `FK99F7D287A7B1F0B0` (`dcr_id`),
  CONSTRAINT `FK99F7D287264F3B1D` FOREIGN KEY (`tdi_id`) REFERENCES `tipos_disciplina` (`tdi_id`),
  CONSTRAINT `FK99F7D28767EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK99F7D287A7B1F0B0` FOREIGN KEY (`dcr_id`) REFERENCES `divisoes_credito` (`dcr_id`)
) ENGINE=InnoDB AUTO_INCREMENT=47 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `disciplinas`
--

/*!40000 ALTER TABLE `disciplinas` DISABLE KEYS */;
INSERT INTO `disciplinas` (`dis_id`,`dis_codigo`,`dis_cred_pratico`,`dis_cred_teorico`,`dificuldade`,`dis_laboratorio`,`dis_max_alun_pratico`,`dis_max_alun_teorico`,`dis_nome`,`version`,`cen_id`,`dcr_id`,`tdi_id`) VALUES 
 (37,'1',0,2,0,0x00,0,40,'Disciplina1',5,1,NULL,1),
 (38,'2',0,2,0,0x00,0,40,'Disciplina2',3,1,NULL,1),
 (39,'3',0,2,0,0x00,0,40,'Disciplina3',3,1,NULL,1),
 (40,'4',0,2,0,0x00,0,40,'Disciplina4',3,1,NULL,1),
 (41,'5',0,2,0,0x00,0,40,'Disciplina5',3,1,NULL,1),
 (42,'6',0,2,0,0x00,0,40,'Disciplina6',0,1,NULL,1),
 (43,'7',0,2,0,0x00,0,40,'Disciplina7',0,1,NULL,1),
 (44,'8',0,2,0,0x00,0,40,'Disciplina8',0,1,NULL,1),
 (45,'9',0,2,0,0x00,0,40,'Disciplina9',0,1,NULL,1),
 (46,'10',0,2,0,0x00,0,40,'Disciplina10',0,1,NULL,1);
/*!40000 ALTER TABLE `disciplinas` ENABLE KEYS */;


--
-- Definition of table `divisoes_credito`
--

DROP TABLE IF EXISTS `divisoes_credito`;
CREATE TABLE `divisoes_credito` (
  `dcr_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `drc_creditos` int(11) NOT NULL,
  `drc_dia1` int(11) NOT NULL,
  `drc_dia2` int(11) NOT NULL,
  `drc_dia3` int(11) NOT NULL,
  `drc_dia4` int(11) NOT NULL,
  `drc_dia5` int(11) NOT NULL,
  `drc_dia6` int(11) NOT NULL,
  `drc_dia7` int(11) NOT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`dcr_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `divisoes_credito`
--

/*!40000 ALTER TABLE `divisoes_credito` DISABLE KEYS */;
/*!40000 ALTER TABLE `divisoes_credito` ENABLE KEYS */;


--
-- Definition of table `divisoes_credito_cenario`
--

DROP TABLE IF EXISTS `divisoes_credito_cenario`;
CREATE TABLE `divisoes_credito_cenario` (
  `divisoes_credito` bigint(20) NOT NULL,
  `cenario` bigint(20) NOT NULL,
  PRIMARY KEY (`divisoes_credito`,`cenario`),
  KEY `FKF589087DA8E47702` (`divisoes_credito`),
  KEY `FKF589087DE0D9840A` (`cenario`),
  CONSTRAINT `FKF589087DA8E47702` FOREIGN KEY (`divisoes_credito`) REFERENCES `divisoes_credito` (`dcr_id`),
  CONSTRAINT `FKF589087DE0D9840A` FOREIGN KEY (`cenario`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `divisoes_credito_cenario`
--

/*!40000 ALTER TABLE `divisoes_credito_cenario` DISABLE KEYS */;
/*!40000 ALTER TABLE `divisoes_credito_cenario` ENABLE KEYS */;


--
-- Definition of table `equivalencias`
--

DROP TABLE IF EXISTS `equivalencias`;
CREATE TABLE `equivalencias` (
  `eqv_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` int(11) DEFAULT NULL,
  `dis_cursou_id` bigint(20) NOT NULL,
  PRIMARY KEY (`eqv_id`),
  KEY `FK7554F96240BA10F8` (`dis_cursou_id`),
  CONSTRAINT `FK7554F96240BA10F8` FOREIGN KEY (`dis_cursou_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `equivalencias`
--

/*!40000 ALTER TABLE `equivalencias` DISABLE KEYS */;
/*!40000 ALTER TABLE `equivalencias` ENABLE KEYS */;


--
-- Definition of table `equivalencias_elimina`
--

DROP TABLE IF EXISTS `equivalencias_elimina`;
CREATE TABLE `equivalencias_elimina` (
  `eliminada_por` bigint(20) NOT NULL,
  `elimina` bigint(20) NOT NULL,
  PRIMARY KEY (`eliminada_por`,`elimina`),
  KEY `FK5D29AA14D7560399` (`elimina`),
  KEY `FK5D29AA146F26E52F` (`eliminada_por`),
  CONSTRAINT `FK5D29AA146F26E52F` FOREIGN KEY (`eliminada_por`) REFERENCES `equivalencias` (`eqv_id`),
  CONSTRAINT `FK5D29AA14D7560399` FOREIGN KEY (`elimina`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `equivalencias_elimina`
--

/*!40000 ALTER TABLE `equivalencias_elimina` DISABLE KEYS */;
/*!40000 ALTER TABLE `equivalencias_elimina` ENABLE KEYS */;


--
-- Definition of table `fixacoes`
--

DROP TABLE IF EXISTS `fixacoes`;
CREATE TABLE `fixacoes` (
  `fix_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `fix_codigo` varchar(50) NOT NULL,
  `fix_descricao` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `cam_id` bigint(20) DEFAULT NULL,
  `dis_id` bigint(20) DEFAULT NULL,
  `prf_id` bigint(20) DEFAULT NULL,
  `sal_id` bigint(20) DEFAULT NULL,
  `uni_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`fix_id`),
  KEY `FKE8714F06AD343A9B` (`sal_id`),
  KEY `FKE8714F065121F2A6` (`cam_id`),
  KEY `FKE8714F0627898F49` (`prf_id`),
  KEY `FKE8714F06EADC7214` (`dis_id`),
  KEY `FKE8714F064E67EEBC` (`uni_id`),
  CONSTRAINT `FKE8714F0627898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`),
  CONSTRAINT `FKE8714F064E67EEBC` FOREIGN KEY (`uni_id`) REFERENCES `unidades` (`uni_id`),
  CONSTRAINT `FKE8714F065121F2A6` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`),
  CONSTRAINT `FKE8714F06AD343A9B` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`),
  CONSTRAINT `FKE8714F06EADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `fixacoes`
--

/*!40000 ALTER TABLE `fixacoes` DISABLE KEYS */;
/*!40000 ALTER TABLE `fixacoes` ENABLE KEYS */;


--
-- Definition of table `grupos_sala`
--

DROP TABLE IF EXISTS `grupos_sala`;
CREATE TABLE `grupos_sala` (
  `grs_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `grs_codigo` varchar(20) NOT NULL,
  `grs_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `uni_id` bigint(20) NOT NULL,
  PRIMARY KEY (`grs_id`),
  UNIQUE KEY `grs_codigo` (`grs_codigo`,`uni_id`),
  KEY `FKA00422384E67EEBC` (`uni_id`),
  CONSTRAINT `FKA00422384E67EEBC` FOREIGN KEY (`uni_id`) REFERENCES `unidades` (`uni_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `grupos_sala`
--

/*!40000 ALTER TABLE `grupos_sala` DISABLE KEYS */;
/*!40000 ALTER TABLE `grupos_sala` ENABLE KEYS */;


--
-- Definition of table `grupos_sala_curriculo_disciplinas`
--

DROP TABLE IF EXISTS `grupos_sala_curriculo_disciplinas`;
CREATE TABLE `grupos_sala_curriculo_disciplinas` (
  `grupos_sala` bigint(20) NOT NULL,
  `curriculo_disciplinas` bigint(20) NOT NULL,
  PRIMARY KEY (`grupos_sala`,`curriculo_disciplinas`),
  KEY `FK286E0A6D1A84B2C8` (`grupos_sala`),
  KEY `FK286E0A6D643CDB70` (`curriculo_disciplinas`),
  CONSTRAINT `FK286E0A6D1A84B2C8` FOREIGN KEY (`grupos_sala`) REFERENCES `grupos_sala` (`grs_id`),
  CONSTRAINT `FK286E0A6D643CDB70` FOREIGN KEY (`curriculo_disciplinas`) REFERENCES `curriculos_disciplinas` (`cdi_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `grupos_sala_curriculo_disciplinas`
--

/*!40000 ALTER TABLE `grupos_sala_curriculo_disciplinas` DISABLE KEYS */;
/*!40000 ALTER TABLE `grupos_sala_curriculo_disciplinas` ENABLE KEYS */;


--
-- Definition of table `grupos_sala_salas`
--

DROP TABLE IF EXISTS `grupos_sala_salas`;
CREATE TABLE `grupos_sala_salas` (
  `grupos_sala` bigint(20) NOT NULL,
  `salas` bigint(20) NOT NULL,
  PRIMARY KEY (`grupos_sala`,`salas`),
  KEY `FK5BDE43C9E9EFC20F` (`salas`),
  KEY `FK5BDE43C91A84B2C8` (`grupos_sala`),
  CONSTRAINT `FK5BDE43C91A84B2C8` FOREIGN KEY (`grupos_sala`) REFERENCES `grupos_sala` (`grs_id`),
  CONSTRAINT `FK5BDE43C9E9EFC20F` FOREIGN KEY (`salas`) REFERENCES `salas` (`sal_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `grupos_sala_salas`
--

/*!40000 ALTER TABLE `grupos_sala_salas` DISABLE KEYS */;
/*!40000 ALTER TABLE `grupos_sala_salas` ENABLE KEYS */;


--
-- Definition of table `horario_disponivel_cenario`
--

DROP TABLE IF EXISTS `horario_disponivel_cenario`;
CREATE TABLE `horario_disponivel_cenario` (
  `hdc_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `semana` int(11) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `hor_id` bigint(20) NOT NULL,
  PRIMARY KEY (`hdc_id`),
  KEY `FK945A41B63E60F87E` (`hor_id`),
  CONSTRAINT `FK945A41B63E60F87E` FOREIGN KEY (`hor_id`) REFERENCES `horarios_aula` (`hor_id`)
) ENGINE=InnoDB AUTO_INCREMENT=158 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horario_disponivel_cenario`
--

/*!40000 ALTER TABLE `horario_disponivel_cenario` DISABLE KEYS */;
INSERT INTO `horario_disponivel_cenario` (`hdc_id`,`semana`,`version`,`hor_id`) VALUES 
 (138,0,9,9),
 (139,1,9,9),
 (140,2,9,9),
 (141,3,9,9),
 (142,4,9,9),
 (143,0,9,10),
 (144,1,9,10),
 (145,2,9,10),
 (146,3,9,10),
 (147,4,9,10),
 (148,0,9,11),
 (149,1,9,11),
 (150,2,9,11),
 (151,3,9,11),
 (152,4,9,11),
 (153,0,9,12),
 (154,1,9,12),
 (155,2,9,12),
 (156,3,9,12),
 (157,4,9,12);
/*!40000 ALTER TABLE `horario_disponivel_cenario` ENABLE KEYS */;


--
-- Definition of table `horario_disponivel_cenario_campi`
--

DROP TABLE IF EXISTS `horario_disponivel_cenario_campi`;
CREATE TABLE `horario_disponivel_cenario_campi` (
  `horarios` bigint(20) NOT NULL,
  `campi` bigint(20) NOT NULL,
  PRIMARY KEY (`horarios`,`campi`),
  KEY `FK2150DA3FA8491D23` (`campi`),
  KEY `FK2150DA3FE2658CD9` (`horarios`),
  CONSTRAINT `FK2150DA3FA8491D23` FOREIGN KEY (`campi`) REFERENCES `campi` (`cam_id`),
  CONSTRAINT `FK2150DA3FE2658CD9` FOREIGN KEY (`horarios`) REFERENCES `horario_disponivel_cenario` (`hdc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horario_disponivel_cenario_campi`
--

/*!40000 ALTER TABLE `horario_disponivel_cenario_campi` DISABLE KEYS */;
INSERT INTO `horario_disponivel_cenario_campi` (`horarios`,`campi`) VALUES 
 (138,8),
 (139,8),
 (140,8),
 (141,8),
 (142,8),
 (143,8),
 (144,8),
 (145,8),
 (146,8),
 (147,8),
 (148,8),
 (149,8),
 (150,8),
 (151,8),
 (152,8),
 (153,8),
 (154,8),
 (155,8),
 (156,8),
 (157,8);
/*!40000 ALTER TABLE `horario_disponivel_cenario_campi` ENABLE KEYS */;


--
-- Definition of table `horario_disponivel_cenario_disciplinas`
--

DROP TABLE IF EXISTS `horario_disponivel_cenario_disciplinas`;
CREATE TABLE `horario_disponivel_cenario_disciplinas` (
  `horarios` bigint(20) NOT NULL,
  `disciplinas` bigint(20) NOT NULL,
  PRIMARY KEY (`horarios`,`disciplinas`),
  KEY `FK280B917EE2658CD9` (`horarios`),
  KEY `FK280B917ED4324A2F` (`disciplinas`),
  CONSTRAINT `FK280B917ED4324A2F` FOREIGN KEY (`disciplinas`) REFERENCES `disciplinas` (`dis_id`),
  CONSTRAINT `FK280B917EE2658CD9` FOREIGN KEY (`horarios`) REFERENCES `horario_disponivel_cenario` (`hdc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horario_disponivel_cenario_disciplinas`
--

/*!40000 ALTER TABLE `horario_disponivel_cenario_disciplinas` DISABLE KEYS */;
INSERT INTO `horario_disponivel_cenario_disciplinas` (`horarios`,`disciplinas`) VALUES 
 (138,37),
 (138,38),
 (138,39),
 (138,40),
 (138,41),
 (138,42),
 (138,43),
 (138,44),
 (138,45),
 (138,46),
 (139,37),
 (139,38),
 (139,39),
 (139,40),
 (139,41),
 (139,42),
 (139,43),
 (139,44),
 (139,45),
 (139,46),
 (140,37),
 (140,38),
 (140,39),
 (140,40),
 (140,41),
 (140,42),
 (140,43),
 (140,44),
 (140,45),
 (140,46),
 (141,37),
 (141,38),
 (141,39),
 (141,40),
 (141,41),
 (141,42),
 (141,43),
 (141,44),
 (141,45),
 (141,46),
 (142,37),
 (142,38),
 (142,39),
 (142,40),
 (142,41),
 (142,42),
 (142,43),
 (142,44),
 (142,45),
 (142,46),
 (143,37),
 (143,38),
 (143,39),
 (143,40),
 (143,41),
 (143,42),
 (143,43),
 (143,44),
 (143,45),
 (143,46),
 (144,37),
 (144,38),
 (144,39),
 (144,40),
 (144,41),
 (144,42),
 (144,43),
 (144,44),
 (144,45),
 (144,46),
 (145,37),
 (145,38),
 (145,39),
 (145,40),
 (145,41),
 (145,42),
 (145,43),
 (145,44),
 (145,45),
 (145,46),
 (146,37),
 (146,38),
 (146,39),
 (146,40),
 (146,41),
 (146,42),
 (146,43),
 (146,44),
 (146,45),
 (146,46),
 (147,37),
 (147,38),
 (147,39),
 (147,40),
 (147,41),
 (147,42),
 (147,43),
 (147,44),
 (147,45),
 (147,46),
 (148,37),
 (148,38),
 (148,39),
 (148,40),
 (148,41),
 (148,42),
 (148,43),
 (148,44),
 (148,45),
 (148,46),
 (149,37),
 (149,38),
 (149,39),
 (149,40),
 (149,41),
 (149,42),
 (149,43),
 (149,44),
 (149,45),
 (149,46),
 (150,37),
 (150,38),
 (150,39),
 (150,40),
 (150,41),
 (150,42),
 (150,43),
 (150,44),
 (150,45),
 (150,46),
 (151,37),
 (151,38),
 (151,39),
 (151,40),
 (151,41),
 (151,42),
 (151,43),
 (151,44),
 (151,45),
 (151,46),
 (152,37),
 (152,38),
 (152,39),
 (152,40),
 (152,41),
 (152,42),
 (152,43),
 (152,44),
 (152,45),
 (152,46),
 (153,37),
 (153,38),
 (153,39),
 (153,40),
 (153,41),
 (153,42),
 (153,43),
 (153,44),
 (153,45),
 (153,46),
 (154,37),
 (154,38),
 (154,39),
 (154,40),
 (154,41),
 (154,42),
 (154,43),
 (154,44),
 (154,45),
 (154,46),
 (155,37),
 (155,38),
 (155,39),
 (155,40),
 (155,41),
 (155,42),
 (155,43),
 (155,44),
 (155,45),
 (155,46),
 (156,37),
 (156,38),
 (156,39),
 (156,40),
 (156,41),
 (156,42),
 (156,43),
 (156,44),
 (156,45),
 (156,46),
 (157,37),
 (157,38),
 (157,39),
 (157,40),
 (157,41),
 (157,42),
 (157,43),
 (157,44),
 (157,45),
 (157,46);
/*!40000 ALTER TABLE `horario_disponivel_cenario_disciplinas` ENABLE KEYS */;


--
-- Definition of table `horario_disponivel_cenario_fixacoes`
--

DROP TABLE IF EXISTS `horario_disponivel_cenario_fixacoes`;
CREATE TABLE `horario_disponivel_cenario_fixacoes` (
  `horarios` bigint(20) NOT NULL,
  `fixacoes` bigint(20) NOT NULL,
  PRIMARY KEY (`horarios`,`fixacoes`),
  KEY `FKF0ABCB6FE2658CD9` (`horarios`),
  KEY `FKF0ABCB6F47BF4DAF` (`fixacoes`),
  CONSTRAINT `FKF0ABCB6F47BF4DAF` FOREIGN KEY (`fixacoes`) REFERENCES `fixacoes` (`fix_id`),
  CONSTRAINT `FKF0ABCB6FE2658CD9` FOREIGN KEY (`horarios`) REFERENCES `horario_disponivel_cenario` (`hdc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horario_disponivel_cenario_fixacoes`
--

/*!40000 ALTER TABLE `horario_disponivel_cenario_fixacoes` DISABLE KEYS */;
/*!40000 ALTER TABLE `horario_disponivel_cenario_fixacoes` ENABLE KEYS */;


--
-- Definition of table `horario_disponivel_cenario_professores`
--

DROP TABLE IF EXISTS `horario_disponivel_cenario_professores`;
CREATE TABLE `horario_disponivel_cenario_professores` (
  `horarios` bigint(20) NOT NULL,
  `professores` bigint(20) NOT NULL,
  PRIMARY KEY (`horarios`,`professores`),
  KEY `FKD1D5C254A5B68610` (`professores`),
  KEY `FKD1D5C254E2658CD9` (`horarios`),
  CONSTRAINT `FKD1D5C254A5B68610` FOREIGN KEY (`professores`) REFERENCES `professores` (`prf_id`),
  CONSTRAINT `FKD1D5C254E2658CD9` FOREIGN KEY (`horarios`) REFERENCES `horario_disponivel_cenario` (`hdc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horario_disponivel_cenario_professores`
--

/*!40000 ALTER TABLE `horario_disponivel_cenario_professores` DISABLE KEYS */;
/*!40000 ALTER TABLE `horario_disponivel_cenario_professores` ENABLE KEYS */;


--
-- Definition of table `horario_disponivel_cenario_salas`
--

DROP TABLE IF EXISTS `horario_disponivel_cenario_salas`;
CREATE TABLE `horario_disponivel_cenario_salas` (
  `horarios` bigint(20) NOT NULL,
  `salas` bigint(20) NOT NULL,
  PRIMARY KEY (`horarios`,`salas`),
  KEY `FK22324CC7E2658CD9` (`horarios`),
  KEY `FK22324CC7E9EFC20F` (`salas`),
  CONSTRAINT `FK22324CC7E2658CD9` FOREIGN KEY (`horarios`) REFERENCES `horario_disponivel_cenario` (`hdc_id`),
  CONSTRAINT `FK22324CC7E9EFC20F` FOREIGN KEY (`salas`) REFERENCES `salas` (`sal_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horario_disponivel_cenario_salas`
--

/*!40000 ALTER TABLE `horario_disponivel_cenario_salas` DISABLE KEYS */;
INSERT INTO `horario_disponivel_cenario_salas` (`horarios`,`salas`) VALUES 
 (138,18),
 (139,18),
 (140,18),
 (141,18),
 (142,18),
 (143,18),
 (144,18),
 (145,18),
 (146,18),
 (147,18),
 (148,18),
 (149,18),
 (150,18),
 (151,18),
 (152,18),
 (153,18),
 (154,18),
 (155,18),
 (156,18),
 (157,18);
/*!40000 ALTER TABLE `horario_disponivel_cenario_salas` ENABLE KEYS */;


--
-- Definition of table `horario_disponivel_cenario_unidades`
--

DROP TABLE IF EXISTS `horario_disponivel_cenario_unidades`;
CREATE TABLE `horario_disponivel_cenario_unidades` (
  `horarios` bigint(20) NOT NULL,
  `unidades` bigint(20) NOT NULL,
  PRIMARY KEY (`horarios`,`unidades`),
  KEY `FKF5ED17EEE2658CD9` (`horarios`),
  KEY `FKF5ED17EE6E344377` (`unidades`),
  CONSTRAINT `FKF5ED17EE6E344377` FOREIGN KEY (`unidades`) REFERENCES `unidades` (`uni_id`),
  CONSTRAINT `FKF5ED17EEE2658CD9` FOREIGN KEY (`horarios`) REFERENCES `horario_disponivel_cenario` (`hdc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horario_disponivel_cenario_unidades`
--

/*!40000 ALTER TABLE `horario_disponivel_cenario_unidades` DISABLE KEYS */;
INSERT INTO `horario_disponivel_cenario_unidades` (`horarios`,`unidades`) VALUES 
 (138,6),
 (139,6),
 (140,6),
 (141,6),
 (142,6),
 (143,6),
 (144,6),
 (145,6),
 (146,6),
 (147,6),
 (148,6),
 (149,6),
 (150,6),
 (151,6),
 (152,6),
 (153,6),
 (154,6),
 (155,6),
 (156,6),
 (157,6);
/*!40000 ALTER TABLE `horario_disponivel_cenario_unidades` ENABLE KEYS */;


--
-- Definition of table `horarios_aula`
--

DROP TABLE IF EXISTS `horarios_aula`;
CREATE TABLE `horarios_aula` (
  `hor_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `hor_horario` datetime NOT NULL,
  `version` int(11) DEFAULT NULL,
  `sle_id` bigint(20) NOT NULL,
  `tur_id` bigint(20) NOT NULL,
  PRIMARY KEY (`hor_id`),
  KEY `FK743B3B5756F127DF` (`tur_id`),
  KEY `FK743B3B575486001C` (`sle_id`),
  CONSTRAINT `FK743B3B575486001C` FOREIGN KEY (`sle_id`) REFERENCES `semana_letiva` (`sle_id`),
  CONSTRAINT `FK743B3B5756F127DF` FOREIGN KEY (`tur_id`) REFERENCES `turnos` (`tur_id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horarios_aula`
--

/*!40000 ALTER TABLE `horarios_aula` DISABLE KEYS */;
INSERT INTO `horarios_aula` (`hor_id`,`hor_horario`,`version`,`sle_id`,`tur_id`) VALUES 
 (9,'2011-04-06 08:00:00',2,2,6),
 (10,'2011-04-06 08:50:00',2,2,6),
 (11,'2011-04-06 09:50:00',2,2,6),
 (12,'2011-04-06 10:40:00',4,2,6);
/*!40000 ALTER TABLE `horarios_aula` ENABLE KEYS */;


--
-- Definition of table `incompatibilidades`
--

DROP TABLE IF EXISTS `incompatibilidades`;
CREATE TABLE `incompatibilidades` (
  `inc_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` int(11) DEFAULT NULL,
  `dis1_id` bigint(20) NOT NULL,
  `dis2_id` bigint(20) NOT NULL,
  PRIMARY KEY (`inc_id`),
  KEY `FKFABB4ACF9DC3317E` (`dis2_id`),
  KEY `FKFABB4ACF9DC2BD1F` (`dis1_id`),
  CONSTRAINT `FKFABB4ACF9DC2BD1F` FOREIGN KEY (`dis1_id`) REFERENCES `disciplinas` (`dis_id`),
  CONSTRAINT `FKFABB4ACF9DC3317E` FOREIGN KEY (`dis2_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `incompatibilidades`
--

/*!40000 ALTER TABLE `incompatibilidades` DISABLE KEYS */;
/*!40000 ALTER TABLE `incompatibilidades` ENABLE KEYS */;


--
-- Definition of table `ofertas`
--

DROP TABLE IF EXISTS `ofertas`;
CREATE TABLE `ofertas` (
  `cac_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` int(11) DEFAULT NULL,
  `cam_id` bigint(20) NOT NULL,
  `crc_id` bigint(20) NOT NULL,
  `tur_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cac_id`),
  KEY `FKA3A6D8625121F2A6` (`cam_id`),
  KEY `FKA3A6D86256F127DF` (`tur_id`),
  KEY `FKA3A6D86274E10E16` (`crc_id`),
  CONSTRAINT `FKA3A6D8625121F2A6` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`),
  CONSTRAINT `FKA3A6D86256F127DF` FOREIGN KEY (`tur_id`) REFERENCES `turnos` (`tur_id`),
  CONSTRAINT `FKA3A6D86274E10E16` FOREIGN KEY (`crc_id`) REFERENCES `curriculos` (`crc_id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ofertas`
--

/*!40000 ALTER TABLE `ofertas` DISABLE KEYS */;
INSERT INTO `ofertas` (`cac_id`,`version`,`cam_id`,`crc_id`,`tur_id`) VALUES 
 (10,2,8,11,6);
/*!40000 ALTER TABLE `ofertas` ENABLE KEYS */;


--
-- Definition of table `parametros`
--

DROP TABLE IF EXISTS `parametros`;
CREATE TABLE `parametros` (
  `par_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `par_alunoperiodosala` bit(1) DEFAULT NULL,
  `par_alunomuitoscampi` bit(1) DEFAULT NULL,
  `par_areatitprofcur` bit(1) DEFAULT NULL,
  `par_avaliacaodesemprof` bit(1) DEFAULT NULL,
  `par_cargahorariaaluno` bit(1) DEFAULT NULL,
  `par_cargahorariaalunosel` varchar(20) DEFAULT NULL,
  `par_cargahorariaprof` bit(1) DEFAULT NULL,
  `par_cargahorariaprofsel` varchar(20) DEFAULT NULL,
  `par_compdisccampi` bit(1) DEFAULT NULL,
  `par_compatdiscdia` bit(1) DEFAULT NULL,
  `par_considerar_equiv` bit(1) DEFAULT NULL,
  `par_evitarredcargahorprof` bit(1) DEFAULT NULL,
  `par_evitarredcargahorprofvalue` int(11) DEFAULT NULL,
  `par_evitarultimoprihorprof` bit(1) DEFAULT NULL,
  `par_funcaoobjetivo` int(11) DEFAULT NULL,
  `par_limmaxdiscpro` bit(1) DEFAULT NULL,
  `par_maxnotaavalcordoc` bit(1) DEFAULT NULL,
  `par_minalunturma` bit(1) DEFAULT NULL,
  `par_minalunturmavalue` int(11) DEFAULT NULL,
  `par_mincustdoccur` bit(1) DEFAULT NULL,
  `par_mindeslaluno` bit(1) DEFAULT NULL,
  `par_mindeslprof` bit(1) DEFAULT NULL,
  `par_mindeslprofvalue` int(11) DEFAULT NULL,
  `par_mingapprof` bit(1) DEFAULT NULL,
  `par_modootimizacao` varchar(20) DEFAULT NULL,
  `par_niveldifdisci` bit(1) DEFAULT NULL,
  `par_percmindout` bit(1) DEFAULT NULL,
  `par_percminmest` bit(1) DEFAULT NULL,
  `par_prefprof` bit(1) DEFAULT NULL,
  `par_profmuitoscampi` bit(1) DEFAULT NULL,
  `par_regrasespdivcred` bit(1) DEFAULT NULL,
  `par_regrasgendivcre` bit(1) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `cam_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) NOT NULL,
  `sem_id` bigint(20) NOT NULL,
  `tur_id` bigint(20) NOT NULL,
  PRIMARY KEY (`par_id`),
  KEY `FK1B57F25A67EDB695` (`cen_id`),
  KEY `FK1B57F25A5121F2A6` (`cam_id`),
  KEY `FK1B57F25A5426FE8D` (`sem_id`),
  KEY `FK1B57F25A56F127DF` (`tur_id`),
  CONSTRAINT `FK1B57F25A5121F2A6` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`),
  CONSTRAINT `FK1B57F25A5426FE8D` FOREIGN KEY (`sem_id`) REFERENCES `semana_letiva` (`sle_id`),
  CONSTRAINT `FK1B57F25A56F127DF` FOREIGN KEY (`tur_id`) REFERENCES `turnos` (`tur_id`),
  CONSTRAINT `FK1B57F25A67EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=50 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parametros`
--

/*!40000 ALTER TABLE `parametros` DISABLE KEYS */;
INSERT INTO `parametros` (`par_id`,`par_alunoperiodosala`,`par_alunomuitoscampi`,`par_areatitprofcur`,`par_avaliacaodesemprof`,`par_cargahorariaaluno`,`par_cargahorariaalunosel`,`par_cargahorariaprof`,`par_cargahorariaprofsel`,`par_compdisccampi`,`par_compatdiscdia`,`par_considerar_equiv`,`par_evitarredcargahorprof`,`par_evitarredcargahorprofvalue`,`par_evitarultimoprihorprof`,`par_funcaoobjetivo`,`par_limmaxdiscpro`,`par_maxnotaavalcordoc`,`par_minalunturma`,`par_minalunturmavalue`,`par_mincustdoccur`,`par_mindeslaluno`,`par_mindeslprof`,`par_mindeslprofvalue`,`par_mingapprof`,`par_modootimizacao`,`par_niveldifdisci`,`par_percmindout`,`par_percminmest`,`par_prefprof`,`par_profmuitoscampi`,`par_regrasespdivcred`,`par_regrasgendivcre`,`version`,`cam_id`,`cen_id`,`sem_id`,`tur_id`) VALUES 
 (3,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (4,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (5,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (6,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (7,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (8,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (9,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (10,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (11,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (12,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (13,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (14,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (15,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (16,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (17,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (18,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (19,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (20,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (21,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (22,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (23,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (24,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (25,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (26,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (27,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (28,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (29,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (30,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (31,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (32,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (33,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,0,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (34,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (35,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (36,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (37,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (38,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (39,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (40,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (41,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (42,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (43,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (44,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (45,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (46,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (47,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (48,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6),
 (49,0x00,0x00,0x00,0x00,0x00,'CONCENTRAR',0x00,'CONCENTRAR',0x00,0x00,0x00,0x00,0,0x00,1,0x00,0x00,0x00,0,0x00,0x00,0x00,0,0x00,'TATICO',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0,8,1,2,6);
/*!40000 ALTER TABLE `parametros` ENABLE KEYS */;


--
-- Definition of table `parametros_cursos_max_nota_aval`
--

DROP TABLE IF EXISTS `parametros_cursos_max_nota_aval`;
CREATE TABLE `parametros_cursos_max_nota_aval` (
  `parametros` bigint(20) NOT NULL,
  `cursos_max_nota_aval` bigint(20) NOT NULL,
  PRIMARY KEY (`parametros`,`cursos_max_nota_aval`),
  UNIQUE KEY `cursos_max_nota_aval` (`cursos_max_nota_aval`),
  KEY `FK35EBA5932C1B0FAE` (`cursos_max_nota_aval`),
  KEY `FK35EBA5932DED53D7` (`parametros`),
  CONSTRAINT `FK35EBA5932C1B0FAE` FOREIGN KEY (`cursos_max_nota_aval`) REFERENCES `cursos` (`cur_id`),
  CONSTRAINT `FK35EBA5932DED53D7` FOREIGN KEY (`parametros`) REFERENCES `parametros` (`par_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parametros_cursos_max_nota_aval`
--

/*!40000 ALTER TABLE `parametros_cursos_max_nota_aval` DISABLE KEYS */;
/*!40000 ALTER TABLE `parametros_cursos_max_nota_aval` ENABLE KEYS */;


--
-- Definition of table `parametros_cursos_min_cust`
--

DROP TABLE IF EXISTS `parametros_cursos_min_cust`;
CREATE TABLE `parametros_cursos_min_cust` (
  `parametros` bigint(20) NOT NULL,
  `cursos_min_cust` bigint(20) NOT NULL,
  PRIMARY KEY (`parametros`,`cursos_min_cust`),
  UNIQUE KEY `cursos_min_cust` (`cursos_min_cust`),
  KEY `FKD76440232EC05AE8` (`cursos_min_cust`),
  KEY `FKD76440232DED53D7` (`parametros`),
  CONSTRAINT `FKD76440232DED53D7` FOREIGN KEY (`parametros`) REFERENCES `parametros` (`par_id`),
  CONSTRAINT `FKD76440232EC05AE8` FOREIGN KEY (`cursos_min_cust`) REFERENCES `cursos` (`cur_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parametros_cursos_min_cust`
--

/*!40000 ALTER TABLE `parametros_cursos_min_cust` DISABLE KEYS */;
/*!40000 ALTER TABLE `parametros_cursos_min_cust` ENABLE KEYS */;


--
-- Definition of table `professores`
--

DROP TABLE IF EXISTS `professores`;
CREATE TABLE `professores` (
  `prf_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `prf_ch_max` int(11) DEFAULT NULL,
  `prf_ch_min` int(11) DEFAULT NULL,
  `prf_cpf` varchar(14) NOT NULL,
  `prf_cred_anterior` int(11) DEFAULT NULL,
  `prf_nome` varchar(20) NOT NULL,
  `prf_valor_credito` double DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `ati_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) NOT NULL,
  `tco_id` bigint(20) NOT NULL,
  `tit_id` bigint(20) NOT NULL,
  PRIMARY KEY (`prf_id`),
  UNIQUE KEY `prf_cpf` (`prf_cpf`,`cen_id`),
  KEY `FK43C2035D67EDB695` (`cen_id`),
  KEY `FK43C2035DFA7DB545` (`tit_id`),
  KEY `FK43C2035D563B3986` (`tco_id`),
  KEY `FK43C2035DED717FE1` (`ati_id`),
  CONSTRAINT `FK43C2035D563B3986` FOREIGN KEY (`tco_id`) REFERENCES `tipos_contrato` (`tco_id`),
  CONSTRAINT `FK43C2035D67EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK43C2035DED717FE1` FOREIGN KEY (`ati_id`) REFERENCES `areas_titulacao` (`ati_id`),
  CONSTRAINT `FK43C2035DFA7DB545` FOREIGN KEY (`tit_id`) REFERENCES `titulacoes` (`tit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `professores`
--

/*!40000 ALTER TABLE `professores` DISABLE KEYS */;
/*!40000 ALTER TABLE `professores` ENABLE KEYS */;


--
-- Definition of table `professores_campi`
--

DROP TABLE IF EXISTS `professores_campi`;
CREATE TABLE `professores_campi` (
  `professores` bigint(20) NOT NULL,
  `campi` bigint(20) NOT NULL,
  PRIMARY KEY (`professores`,`campi`),
  KEY `FK7C5482A6A8491D23` (`campi`),
  KEY `FK7C5482A6A5B68610` (`professores`),
  CONSTRAINT `FK7C5482A6A5B68610` FOREIGN KEY (`professores`) REFERENCES `professores` (`prf_id`),
  CONSTRAINT `FK7C5482A6A8491D23` FOREIGN KEY (`campi`) REFERENCES `campi` (`cam_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `professores_campi`
--

/*!40000 ALTER TABLE `professores_campi` DISABLE KEYS */;
/*!40000 ALTER TABLE `professores_campi` ENABLE KEYS */;


--
-- Definition of table `professores_disciplinas`
--

DROP TABLE IF EXISTS `professores_disciplinas`;
CREATE TABLE `professores_disciplinas` (
  `dip_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `prf_nota` int(11) NOT NULL,
  `prf_preferencia` int(11) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `dis_id` bigint(20) NOT NULL,
  `prf_id` bigint(20) NOT NULL,
  PRIMARY KEY (`dip_id`),
  KEY `FK545310A527898F49` (`prf_id`),
  KEY `FK545310A5EADC7214` (`dis_id`),
  CONSTRAINT `FK545310A527898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`),
  CONSTRAINT `FK545310A5EADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `professores_disciplinas`
--

/*!40000 ALTER TABLE `professores_disciplinas` DISABLE KEYS */;
/*!40000 ALTER TABLE `professores_disciplinas` ENABLE KEYS */;


--
-- Definition of table `professores_virtuais`
--

DROP TABLE IF EXISTS `professores_virtuais`;
CREATE TABLE `professores_virtuais` (
  `prf_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `prf_ch_max` int(11) DEFAULT NULL,
  `prf_ch_min` int(11) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `ati_id` bigint(20) DEFAULT NULL,
  `tit_id` bigint(20) NOT NULL,
  PRIMARY KEY (`prf_id`),
  KEY `FKD43F690DFA7DB545` (`tit_id`),
  KEY `FKD43F690DED717FE1` (`ati_id`),
  CONSTRAINT `FKD43F690DED717FE1` FOREIGN KEY (`ati_id`) REFERENCES `areas_titulacao` (`ati_id`),
  CONSTRAINT `FKD43F690DFA7DB545` FOREIGN KEY (`tit_id`) REFERENCES `titulacoes` (`tit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `professores_virtuais`
--

/*!40000 ALTER TABLE `professores_virtuais` DISABLE KEYS */;
/*!40000 ALTER TABLE `professores_virtuais` ENABLE KEYS */;


--
-- Definition of table `professores_virtuais_disciplinas`
--

DROP TABLE IF EXISTS `professores_virtuais_disciplinas`;
CREATE TABLE `professores_virtuais_disciplinas` (
  `professores_virtuais` bigint(20) NOT NULL,
  `disciplinas` bigint(20) NOT NULL,
  PRIMARY KEY (`professores_virtuais`,`disciplinas`),
  KEY `FKBCF56E55D4324A2F` (`disciplinas`),
  KEY `FKBCF56E559208BDE5` (`professores_virtuais`),
  CONSTRAINT `FKBCF56E559208BDE5` FOREIGN KEY (`professores_virtuais`) REFERENCES `professores_virtuais` (`prf_id`),
  CONSTRAINT `FKBCF56E55D4324A2F` FOREIGN KEY (`disciplinas`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `professores_virtuais_disciplinas`
--

/*!40000 ALTER TABLE `professores_virtuais_disciplinas` DISABLE KEYS */;
/*!40000 ALTER TABLE `professores_virtuais_disciplinas` ENABLE KEYS */;


--
-- Definition of table `salas`
--

DROP TABLE IF EXISTS `salas`;
CREATE TABLE `salas` (
  `sal_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `sal_andar` varchar(20) NOT NULL,
  `sal_capacidade` int(11) NOT NULL,
  `sal_codigo` varchar(20) NOT NULL,
  `sal_numero` varchar(20) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `tsa_id` bigint(20) NOT NULL,
  `uni_id` bigint(20) NOT NULL,
  PRIMARY KEY (`sal_id`),
  UNIQUE KEY `uni_id` (`uni_id`,`sal_codigo`),
  KEY `FK6824890F84DC98B` (`tsa_id`),
  KEY `FK68248904E67EEBC` (`uni_id`),
  CONSTRAINT `FK68248904E67EEBC` FOREIGN KEY (`uni_id`) REFERENCES `unidades` (`uni_id`),
  CONSTRAINT `FK6824890F84DC98B` FOREIGN KEY (`tsa_id`) REFERENCES `tipos_sala` (`tsa_id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `salas`
--

/*!40000 ALTER TABLE `salas` DISABLE KEYS */;
INSERT INTO `salas` (`sal_id`,`sal_andar`,`sal_capacidade`,`sal_codigo`,`sal_numero`,`version`,`tsa_id`,`uni_id`) VALUES 
 (18,'1',50,'1','1',2,1,6);
/*!40000 ALTER TABLE `salas` ENABLE KEYS */;


--
-- Definition of table `semana_letiva`
--

DROP TABLE IF EXISTS `semana_letiva`;
CREATE TABLE `semana_letiva` (
  `sle_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `sle_codigo` varchar(20) NOT NULL,
  `sle_descricao` varchar(50) NOT NULL,
  `sle_oficial` bit(1) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`sle_id`),
  UNIQUE KEY `sle_codigo` (`sle_codigo`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `semana_letiva`
--

/*!40000 ALTER TABLE `semana_letiva` DISABLE KEYS */;
INSERT INTO `semana_letiva` (`sle_id`,`sle_codigo`,`sle_descricao`,`sle_oficial`,`version`) VALUES 
 (2,'PADRÃO','Horário Padrão',0x01,22);
/*!40000 ALTER TABLE `semana_letiva` ENABLE KEYS */;


--
-- Definition of table `tipos_contrato`
--

DROP TABLE IF EXISTS `tipos_contrato`;
CREATE TABLE `tipos_contrato` (
  `tco_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tco_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`tco_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tipos_contrato`
--

/*!40000 ALTER TABLE `tipos_contrato` DISABLE KEYS */;
INSERT INTO `tipos_contrato` (`tco_id`,`tco_nome`,`version`) VALUES 
 (1,'Horista',0),
 (2,'Integral',0);
/*!40000 ALTER TABLE `tipos_contrato` ENABLE KEYS */;


--
-- Definition of table `tipos_curso`
--

DROP TABLE IF EXISTS `tipos_curso`;
CREATE TABLE `tipos_curso` (
  `tcu_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tcu_codigo` varchar(50) NOT NULL,
  `tcu_descricao` varchar(255) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`tcu_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tipos_curso`
--

/*!40000 ALTER TABLE `tipos_curso` DISABLE KEYS */;
INSERT INTO `tipos_curso` (`tcu_id`,`tcu_codigo`,`tcu_descricao`,`version`) VALUES 
 (4,'GRAD.','Graduação',0);
/*!40000 ALTER TABLE `tipos_curso` ENABLE KEYS */;


--
-- Definition of table `tipos_disciplina`
--

DROP TABLE IF EXISTS `tipos_disciplina`;
CREATE TABLE `tipos_disciplina` (
  `tdi_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tdi_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`tdi_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tipos_disciplina`
--

/*!40000 ALTER TABLE `tipos_disciplina` DISABLE KEYS */;
INSERT INTO `tipos_disciplina` (`tdi_id`,`tdi_nome`,`version`) VALUES 
 (1,'Presencial',0),
 (2,'Telepresencial',0),
 (3,'Online',0);
/*!40000 ALTER TABLE `tipos_disciplina` ENABLE KEYS */;


--
-- Definition of table `tipos_sala`
--

DROP TABLE IF EXISTS `tipos_sala`;
CREATE TABLE `tipos_sala` (
  `tsa_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tsa_descricao` varchar(255) DEFAULT NULL,
  `tsa_nome` varchar(255) NOT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`tsa_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tipos_sala`
--

/*!40000 ALTER TABLE `tipos_sala` DISABLE KEYS */;
INSERT INTO `tipos_sala` (`tsa_id`,`tsa_descricao`,`tsa_nome`,`version`) VALUES 
 (1,'Sala de Aula','Sala de Aula',0),
 (2,'Laboratório','Laboratório',0),
 (3,'Auditório','Auditório',0);
/*!40000 ALTER TABLE `tipos_sala` ENABLE KEYS */;


--
-- Definition of table `titulacoes`
--

DROP TABLE IF EXISTS `titulacoes`;
CREATE TABLE `titulacoes` (
  `tit_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tit_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`tit_id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `titulacoes`
--

/*!40000 ALTER TABLE `titulacoes` DISABLE KEYS */;
INSERT INTO `titulacoes` (`tit_id`,`tit_nome`,`version`) VALUES 
 (1,'Licenciado',0),
 (2,'Bacharel',0),
 (3,'Especialista',0),
 (4,'Mestre',0),
 (5,'Doutor',0);
/*!40000 ALTER TABLE `titulacoes` ENABLE KEYS */;


--
-- Definition of table `turnos`
--

DROP TABLE IF EXISTS `turnos`;
CREATE TABLE `turnos` (
  `tur_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tur_nome` varchar(50) NOT NULL,
  `tur_tempo` int(11) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  PRIMARY KEY (`tur_id`),
  KEY `FKCC98632167EDB695` (`cen_id`),
  CONSTRAINT `FKCC98632167EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `turnos`
--

/*!40000 ALTER TABLE `turnos` DISABLE KEYS */;
INSERT INTO `turnos` (`tur_id`,`tur_nome`,`tur_tempo`,`version`,`cen_id`) VALUES 
 (6,'Manhã',50,2,1);
/*!40000 ALTER TABLE `turnos` ENABLE KEYS */;


--
-- Definition of table `unidades`
--

DROP TABLE IF EXISTS `unidades`;
CREATE TABLE `unidades` (
  `uni_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `uni_codigo` varchar(20) NOT NULL,
  `uni_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `cam_id` bigint(20) NOT NULL,
  PRIMARY KEY (`uni_id`),
  UNIQUE KEY `cam_id` (`cam_id`,`uni_codigo`),
  KEY `FKEDB29B855121F2A6` (`cam_id`),
  CONSTRAINT `FKEDB29B855121F2A6` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `unidades`
--

/*!40000 ALTER TABLE `unidades` DISABLE KEYS */;
INSERT INTO `unidades` (`uni_id`,`uni_codigo`,`uni_nome`,`version`,`cam_id`) VALUES 
 (6,'1','Unidade1',0,8);
/*!40000 ALTER TABLE `unidades` ENABLE KEYS */;


--
-- Definition of table `users`
--

DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `username` varchar(50) NOT NULL,
  `usu_email` varchar(100) DEFAULT NULL,
  `enabled` bit(1) NOT NULL,
  `usu_nome` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `prf_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`username`),
  UNIQUE KEY `username` (`username`),
  KEY `FK6A68E0827898F49` (`prf_id`),
  CONSTRAINT `FK6A68E0827898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--

/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` (`username`,`usu_email`,`enabled`,`usu_nome`,`password`,`version`,`prf_id`) VALUES 
 ('aaaaa','aaaaa',0x01,'aaaaa','594f803b380a41396ed63dca39503542',0,NULL),
 ('admin','claudio@gapso.com.br',0x01,'Administrador','21232f297a57a5a743894a0e4a801fc3',0,NULL),
 ('prof2','prof2@gapso.com.br',0x01,'Professor 2','c52a44b3378c9ef375b7dadecd783921',0,NULL),
 ('user','',0x01,'','ee11cbb19052e40b07aac0ca060c23ee',0,NULL);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;




/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
