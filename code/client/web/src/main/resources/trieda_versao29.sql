-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.1.50-community


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
-- Definition of table `alunos`
--

DROP TABLE IF EXISTS `alunos`;
CREATE TABLE `alunos` (
  `aln_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `aln_matricula` varchar(50) NOT NULL,
  `aln_nome` varchar(256) NOT NULL,
  `aln_formando` bit(1) NOT NULL DEFAULT b'0',
  `aln_periodo` int(11) DEFAULT NULL,
  `aln_virtual` bit(1) NOT NULL DEFAULT b'0',
  `aln_criado_por_trieda` bit(1) NOT NULL DEFAULT b'0',
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `ins_id` bigint(20) NOT NULL,
  PRIMARY KEY (`aln_id`),
  UNIQUE KEY `aln_matricula` (`aln_matricula`,`cen_id`),
  KEY `FKABAED8E867EDB695` (`cen_id`),
  KEY `FKABAED8E8A432FA9C` (`ins_id`),
  CONSTRAINT `FKABAED8E867EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FKABAED8E8A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`)
) ENGINE=InnoDB AUTO_INCREMENT=41357 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `alunos`
--

/*!40000 ALTER TABLE `alunos` DISABLE KEYS */;
/*!40000 ALTER TABLE `alunos` ENABLE KEYS */;


--
-- Definition of table `alunos_demanda`
--

DROP TABLE IF EXISTS `alunos_demanda`;
CREATE TABLE `alunos_demanda` (
  `ald_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `ald_atendido` bit(1) DEFAULT NULL,
  `ald_prioridade` int(11) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `aln_id` bigint(20) NOT NULL,
  `dem_id` bigint(20) NOT NULL,
  `ald_periodo` int(11) DEFAULT NULL,
  `ald_motivo_nao_atendimento` longtext,
  PRIMARY KEY (`ald_id`),
  UNIQUE KEY `aln_id` (`aln_id`,`dem_id`,`ald_prioridade`) USING BTREE,
  KEY `FK2261771F9E7A2EE8` (`dem_id`),
  KEY `FK2261771F34F4A466` (`aln_id`),
  CONSTRAINT `FK2261771F34F4A466` FOREIGN KEY (`aln_id`) REFERENCES `alunos` (`aln_id`),
  CONSTRAINT `FK2261771F9E7A2EE8` FOREIGN KEY (`dem_id`) REFERENCES `demandas` (`dem_id`)
) ENGINE=InnoDB AUTO_INCREMENT=198211 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `alunos_demanda`
--

/*!40000 ALTER TABLE `alunos_demanda` DISABLE KEYS */;
/*!40000 ALTER TABLE `alunos_demanda` ENABLE KEYS */;


--
-- Definition of table `alunos_demanda_atendimentos_operacional`
--

DROP TABLE IF EXISTS `alunos_demanda_atendimentos_operacional`;
CREATE TABLE `alunos_demanda_atendimentos_operacional` (
  `alunos_demanda` bigint(20) NOT NULL,
  `atendimentos_operacional` bigint(20) NOT NULL,
  PRIMARY KEY (`alunos_demanda`,`atendimentos_operacional`),
  KEY `FK5A2D64C7F2EF1866` (`alunos_demanda`),
  KEY `FK5A2D64C77ACBBB36` (`atendimentos_operacional`),
  CONSTRAINT `FK5A2D64C77ACBBB36` FOREIGN KEY (`atendimentos_operacional`) REFERENCES `atendimento_operacional` (`atp_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK5A2D64C7F2EF1866` FOREIGN KEY (`alunos_demanda`) REFERENCES `alunos_demanda` (`ald_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `alunos_demanda_atendimentos_operacional`
--

/*!40000 ALTER TABLE `alunos_demanda_atendimentos_operacional` DISABLE KEYS */;
/*!40000 ALTER TABLE `alunos_demanda_atendimentos_operacional` ENABLE KEYS */;


--
-- Definition of table `alunos_demanda_atendimentos_tatico`
--

DROP TABLE IF EXISTS `alunos_demanda_atendimentos_tatico`;
CREATE TABLE `alunos_demanda_atendimentos_tatico` (
  `alunos_demanda` bigint(20) NOT NULL,
  `atendimentos_tatico` bigint(20) NOT NULL,
  PRIMARY KEY (`alunos_demanda`,`atendimentos_tatico`),
  KEY `FKDDCB9B8AF2EF1866` (`alunos_demanda`),
  KEY `FKDDCB9B8A51EFDC6C` (`atendimentos_tatico`),
  CONSTRAINT `FKDDCB9B8A51EFDC6C` FOREIGN KEY (`atendimentos_tatico`) REFERENCES `atendimento_tatico` (`att_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FKDDCB9B8AF2EF1866` FOREIGN KEY (`alunos_demanda`) REFERENCES `alunos_demanda` (`ald_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `alunos_demanda_atendimentos_tatico`
--

/*!40000 ALTER TABLE `alunos_demanda_atendimentos_tatico` DISABLE KEYS */;
/*!40000 ALTER TABLE `alunos_demanda_atendimentos_tatico` ENABLE KEYS */;


--
-- Definition of table `areas_titulacao`
--

DROP TABLE IF EXISTS `areas_titulacao`;
CREATE TABLE `areas_titulacao` (
  `ati_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `ati_codigo` varchar(50) NOT NULL,
  `ati_descricao` varchar(255) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `ins_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`ati_id`),
  KEY `FKBF5C9CCDA432FA9C` (`ins_id`),
  KEY `FK_CEN_ID_ATT` (`cen_id`),
  CONSTRAINT `FKBF5C9CCDA432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK_CEN_ID_ATT` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=53 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `areas_titulacao`
--

/*!40000 ALTER TABLE `areas_titulacao` DISABLE KEYS */;
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
  `prf_id` bigint(20) DEFAULT NULL,
  `prv_id` bigint(20) DEFAULT NULL,
  `sal_id` bigint(20) NOT NULL,
  `ins_id` bigint(20) NOT NULL,
  `dis_substituta_id` bigint(20) DEFAULT NULL,
  `atp_confirmada` bit(1) DEFAULT b'0',
  PRIMARY KEY (`atp_id`),
  KEY `FK3C68D5467EDB695` (`cen_id`),
  KEY `FK3C68D54AD343A9B` (`sal_id`),
  KEY `FK3C68D54AE2A797B` (`hdc_id`),
  KEY `FK3C68D548365A75E` (`prv_id`),
  KEY `FK3C68D5427898F49` (`prf_id`),
  KEY `FK3C68D54EADC7214` (`dis_id`),
  KEY `FK3C68D547A9BEDF9` (`ofe_id`),
  KEY `FK3C68D54A432FA9C` (`ins_id`),
  KEY `FK3C68D5466C63B1B` (`dis_substituta_id`),
  CONSTRAINT `FK3C68D5427898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`),
  CONSTRAINT `FK3C68D5466C63B1B` FOREIGN KEY (`dis_substituta_id`) REFERENCES `disciplinas` (`dis_id`),
  CONSTRAINT `FK3C68D5467EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK3C68D547A9BEDF9` FOREIGN KEY (`ofe_id`) REFERENCES `ofertas` (`ofe_id`),
  CONSTRAINT `FK3C68D548365A75E` FOREIGN KEY (`prv_id`) REFERENCES `professores_virtuais` (`prf_id`),
  CONSTRAINT `FK3C68D54A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK3C68D54AD343A9B` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`),
  CONSTRAINT `FK3C68D54AE2A797B` FOREIGN KEY (`hdc_id`) REFERENCES `horario_disponivel_cenario` (`hdc_id`),
  CONSTRAINT `FK3C68D54EADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB AUTO_INCREMENT=761 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `atendimento_operacional`
--

/*!40000 ALTER TABLE `atendimento_operacional` DISABLE KEYS */;
/*!40000 ALTER TABLE `atendimento_operacional` ENABLE KEYS */;


--
-- Definition of table `atendimento_operacional_dicas_eliminacao`
--

DROP TABLE IF EXISTS `atendimento_operacional_dicas_eliminacao`;
CREATE TABLE `atendimento_operacional_dicas_eliminacao` (
  `atp_id` bigint(20) NOT NULL,
  `dic_prv_id` bigint(20) NOT NULL,
  KEY `FK_ATP_DIC_ATP_ID` (`atp_id`),
  KEY `FK_ATP_DIC_DIC_PRV_ID` (`dic_prv_id`),
  CONSTRAINT `FK_ATP_DIC_ATP_ID` FOREIGN KEY (`atp_id`) REFERENCES `atendimento_operacional` (`atp_id`),
  CONSTRAINT `FK_ATP_DIC_DIC_PRV_ID` FOREIGN KEY (`dic_prv_id`) REFERENCES `dicas_eliminacao_prof_virtual` (`dic_prv_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `atendimento_operacional_dicas_eliminacao`
--

/*!40000 ALTER TABLE `atendimento_operacional_dicas_eliminacao` DISABLE KEYS */;
/*!40000 ALTER TABLE `atendimento_operacional_dicas_eliminacao` ENABLE KEYS */;


--
-- Definition of table `atendimento_operacional_motivos_uso`
--

DROP TABLE IF EXISTS `atendimento_operacional_motivos_uso`;
CREATE TABLE `atendimento_operacional_motivos_uso` (
  `atp_id` bigint(20) NOT NULL,
  `mot_prv_id` bigint(20) NOT NULL,
  KEY `FK_ATP_MOT_ATP_ID` (`atp_id`),
  KEY `FK_ATP_MOT_MOT_PRV_ID` (`mot_prv_id`),
  CONSTRAINT `FK_ATP_MOT_ATP_ID` FOREIGN KEY (`atp_id`) REFERENCES `atendimento_operacional` (`atp_id`),
  CONSTRAINT `FK_ATP_MOT_MOT_PRV_ID` FOREIGN KEY (`mot_prv_id`) REFERENCES `motivos_uso_prof_virtual` (`mot_prv_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `atendimento_operacional_motivos_uso`
--

/*!40000 ALTER TABLE `atendimento_operacional_motivos_uso` DISABLE KEYS */;
/*!40000 ALTER TABLE `atendimento_operacional_motivos_uso` ENABLE KEYS */;


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
  `ins_id` bigint(20) NOT NULL,
  `dis_substituta_id` bigint(20) DEFAULT NULL,
  `hor_id` bigint(20) DEFAULT NULL,
  `att_confirmada` bit(1) DEFAULT b'0',
  PRIMARY KEY (`att_id`),
  KEY `FK54748EDD67EDB695` (`cen_id`),
  KEY `FK54748EDDAD343A9B` (`sal_id`),
  KEY `FK54748EDDEADC7214` (`dis_id`),
  KEY `FK54748EDD7A9BEDF9` (`ofe_id`),
  KEY `FK54748EDDA432FA9C` (`ins_id`),
  KEY `FK54748EDD66C63B1B` (`dis_substituta_id`),
  KEY `FK54748EDD3E60F87E` (`hor_id`),
  CONSTRAINT `FK54748EDD3E60F87E` FOREIGN KEY (`hor_id`) REFERENCES `horarios_aula` (`hor_id`),
  CONSTRAINT `FK54748EDD66C63B1B` FOREIGN KEY (`dis_substituta_id`) REFERENCES `disciplinas` (`dis_id`),
  CONSTRAINT `FK54748EDD67EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK54748EDD7A9BEDF9` FOREIGN KEY (`ofe_id`) REFERENCES `ofertas` (`ofe_id`),
  CONSTRAINT `FK54748EDDA432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK54748EDDAD343A9B` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`),
  CONSTRAINT `FK54748EDDEADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB AUTO_INCREMENT=810 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `atendimento_tatico`
--

/*!40000 ALTER TABLE `atendimento_tatico` DISABLE KEYS */;
/*!40000 ALTER TABLE `atendimento_tatico` ENABLE KEYS */;


--
-- Definition of table `atendimentos_faixa_demanda`
--

DROP TABLE IF EXISTS `atendimentos_faixa_demanda`;
CREATE TABLE `atendimentos_faixa_demanda` (
  `afd_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `cam_id` bigint(20) NOT NULL,
  `afd_faixa_sup` int(11) DEFAULT NULL,
  `afd_faixa_inf` int(11) DEFAULT NULL,
  `afd_dem_p1` int(11) DEFAULT NULL,
  `afd_atend_p1` int(11) DEFAULT NULL,
  `afd_atend_percent_p1` double DEFAULT NULL,
  `afd_atend_soma` int(11) DEFAULT NULL,
  `afd_atend_soma_percent` double DEFAULT NULL,
  `afd_turmas_abertas` double DEFAULT NULL,
  `afd_media_turma` double DEFAULT NULL,
  `afd_dem_acum_p1` int(11) DEFAULT NULL,
  `afd_creditos_pagos` double DEFAULT NULL,
  `afd_atend_soma_acum` int(11) DEFAULT NULL,
  `afd_atend_acum_percent` double DEFAULT NULL,
  `afd_receita_semanal` double DEFAULT NULL,
  `afd_custo_docente_semanal` double DEFAULT NULL,
  `afd_custo_docente_percent` double DEFAULT NULL,
  `afd_receita_acum` double DEFAULT NULL,
  `afd_custo_docente_acum` double DEFAULT NULL,
  `afd_custo_docente_acum_percent` double DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`afd_id`),
  KEY `FK_AFD_CAM_ID` (`cam_id`),
  CONSTRAINT `FK_AFD_CAM_ID` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `atendimentos_faixa_demanda`
--

/*!40000 ALTER TABLE `atendimentos_faixa_demanda` DISABLE KEYS */;
/*!40000 ALTER TABLE `atendimentos_faixa_demanda` ENABLE KEYS */;


--
-- Definition of table `authorities`
--

DROP TABLE IF EXISTS `authorities`;
CREATE TABLE `authorities` (
  `username` varchar(50) NOT NULL,
  `authority` varchar(255) DEFAULT NULL,
  `ins_id` bigint(20) NOT NULL,
  PRIMARY KEY (`username`),
  KEY `FK2B0F1321A432FA9C` (`ins_id`),
  CONSTRAINT `FK2B0F1321A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `authorities`
--

/*!40000 ALTER TABLE `authorities` DISABLE KEYS */;
INSERT INTO `authorities` (`username`,`authority`,`ins_id`) VALUES 
 ('admin','ROLE_SUPERVISOR',0);
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
  `ins_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cam_id`),
  UNIQUE KEY `cam_codigo` (`cam_codigo`,`cen_id`),
  KEY `FK5A0D60867EDB695` (`cen_id`),
  KEY `FK5A0D608A432FA9C` (`ins_id`),
  CONSTRAINT `FK5A0D60867EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK5A0D608A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`)
) ENGINE=InnoDB AUTO_INCREMENT=49 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `campi`
--

/*!40000 ALTER TABLE `campi` DISABLE KEYS */;
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
  `ins_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cen_id`),
  KEY `FKCC2B217081EC78EA` (`usu_criacao_id`),
  KEY `FKCC2B2170BE181A06` (`usu_atualizacao_id`),
  KEY `FKCC2B2170A432FA9C` (`ins_id`),
  CONSTRAINT `FKCC2B217081EC78EA` FOREIGN KEY (`usu_criacao_id`) REFERENCES `users` (`username`),
  CONSTRAINT `FKCC2B2170A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FKCC2B2170BE181A06` FOREIGN KEY (`usu_atualizacao_id`) REFERENCES `users` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cenarios`
--

/*!40000 ALTER TABLE `cenarios` DISABLE KEYS */;
INSERT INTO `cenarios` (`cen_id`,`cen_ano`,`cen_comentario`,`cen_dt_atualizacao`,`cen_dt_criacao`,`cen_masterdata`,`cen_nome`,`cen_oficial`,`cen_semestre`,`version`,`usu_atualizacao_id`,`usu_criacao_id`,`ins_id`) VALUES 
 (8,1,'MASTER DATA',NULL,NULL,0x01,'MASTER DATA',0x00,1,16,NULL,NULL,0);
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
  `sle_id` bigint(20) NOT NULL,
  PRIMARY KEY (`crc_id`),
  UNIQUE KEY `crc_cod` (`crc_cod`,`cur_id`),
  KEY `FKC5509B8767EDB695` (`cen_id`),
  KEY `FKC5509B8738FF365A` (`cur_id`),
  KEY `FKC5509B875486001C` (`sle_id`),
  CONSTRAINT `FKC5509B8738FF365A` FOREIGN KEY (`cur_id`) REFERENCES `cursos` (`cur_id`),
  CONSTRAINT `FKC5509B875486001C` FOREIGN KEY (`sle_id`) REFERENCES `semana_letiva` (`sle_id`),
  CONSTRAINT `FKC5509B8767EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=595 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curriculos`
--

/*!40000 ALTER TABLE `curriculos` DISABLE KEYS */;
/*!40000 ALTER TABLE `curriculos` ENABLE KEYS */;


--
-- Definition of table `curriculos_disciplinas`
--

DROP TABLE IF EXISTS `curriculos_disciplinas`;
CREATE TABLE `curriculos_disciplinas` (
  `cdi_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `cdi_periodo` int(11) NOT NULL,
  `cdi_maturidade` int(11) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `crc_id` bigint(20) NOT NULL,
  `dis_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cdi_id`),
  KEY `FKDE3CB9CFEADC7214` (`dis_id`),
  KEY `FKDE3CB9CF74E10E16` (`crc_id`),
  CONSTRAINT `FKDE3CB9CF74E10E16` FOREIGN KEY (`crc_id`) REFERENCES `curriculos` (`crc_id`),
  CONSTRAINT `FKDE3CB9CFEADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB AUTO_INCREMENT=20713 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curriculos_disciplinas`
--

/*!40000 ALTER TABLE `curriculos_disciplinas` DISABLE KEYS */;
/*!40000 ALTER TABLE `curriculos_disciplinas` ENABLE KEYS */;


--
-- Definition of table `curriculos_disciplinas_alunos`
--

DROP TABLE IF EXISTS `curriculos_disciplinas_alunos`;
CREATE TABLE `curriculos_disciplinas_alunos` (
  `cdi_id` bigint(20) NOT NULL,
  `aln_id` bigint(20) NOT NULL,
  KEY `FK_CDI_ALN_CDI_ID` (`cdi_id`),
  KEY `FK_CDI_ALN_ALN_ID` (`aln_id`),
  CONSTRAINT `FK_CDI_ALN_ALN_ID` FOREIGN KEY (`aln_id`) REFERENCES `alunos` (`aln_id`),
  CONSTRAINT `FK_CDI_ALN_CDI_ID` FOREIGN KEY (`cdi_id`) REFERENCES `curriculos_disciplinas` (`cdi_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curriculos_disciplinas_alunos`
--

/*!40000 ALTER TABLE `curriculos_disciplinas_alunos` DISABLE KEYS */;
/*!40000 ALTER TABLE `curriculos_disciplinas_alunos` ENABLE KEYS */;


--
-- Definition of table `curriculos_disciplinas_disciplinas_corequisitos`
--

DROP TABLE IF EXISTS `curriculos_disciplinas_disciplinas_corequisitos`;
CREATE TABLE `curriculos_disciplinas_disciplinas_corequisitos` (
  `cdi_id` bigint(20) NOT NULL,
  `dis_id` bigint(20) NOT NULL,
  KEY `FK_CDI_DIS_CO_CDI_ID` (`cdi_id`),
  KEY `FK_CDI_DIS_CO_DIS_ID` (`dis_id`),
  CONSTRAINT `FK_CDI_DIS_CO_CDI_ID` FOREIGN KEY (`cdi_id`) REFERENCES `curriculos_disciplinas` (`cdi_id`),
  CONSTRAINT `FK_CDI_DIS_CO_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curriculos_disciplinas_disciplinas_corequisitos`
--

/*!40000 ALTER TABLE `curriculos_disciplinas_disciplinas_corequisitos` DISABLE KEYS */;
/*!40000 ALTER TABLE `curriculos_disciplinas_disciplinas_corequisitos` ENABLE KEYS */;


--
-- Definition of table `curriculos_disciplinas_disciplinas_prerequisitos`
--

DROP TABLE IF EXISTS `curriculos_disciplinas_disciplinas_prerequisitos`;
CREATE TABLE `curriculos_disciplinas_disciplinas_prerequisitos` (
  `cdi_id` bigint(20) NOT NULL,
  `dis_id` bigint(20) NOT NULL,
  KEY `FK_CDI_DIS_PRE_CDI_ID` (`cdi_id`),
  KEY `FK_CDI_DIS_PRE_DIS_ID` (`dis_id`),
  CONSTRAINT `FK_CDI_DIS_PRE_CDI_ID` FOREIGN KEY (`cdi_id`) REFERENCES `curriculos_disciplinas` (`cdi_id`),
  CONSTRAINT `FK_CDI_DIS_PRE_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `curriculos_disciplinas_disciplinas_prerequisitos`
--

/*!40000 ALTER TABLE `curriculos_disciplinas_disciplinas_prerequisitos` DISABLE KEYS */;
/*!40000 ALTER TABLE `curriculos_disciplinas_disciplinas_prerequisitos` ENABLE KEYS */;


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
  `cur_nome` varchar(255) DEFAULT NULL,
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
) ENGINE=InnoDB AUTO_INCREMENT=346 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cursos`
--

/*!40000 ALTER TABLE `cursos` DISABLE KEYS */;
/*!40000 ALTER TABLE `cursos` ENABLE KEYS */;


--
-- Definition of table `cursos_equivalencias`
--

DROP TABLE IF EXISTS `cursos_equivalencias`;
CREATE TABLE `cursos_equivalencias` (
  `cur_id` bigint(20) NOT NULL,
  `eqv_id` bigint(20) NOT NULL,
  PRIMARY KEY (`cur_id`,`eqv_id`),
  KEY `FK_CUR_EQV_CUR_ID` (`cur_id`),
  KEY `FK_CUR_EQV_EQV_ID` (`eqv_id`),
  CONSTRAINT `FK_CUR_EQV_CUR_ID` FOREIGN KEY (`cur_id`) REFERENCES `cursos` (`cur_id`),
  CONSTRAINT `FK_CUR_EQV_EQV_ID` FOREIGN KEY (`eqv_id`) REFERENCES `equivalencias` (`eqv_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cursos_equivalencias`
--

/*!40000 ALTER TABLE `cursos_equivalencias` DISABLE KEYS */;
/*!40000 ALTER TABLE `cursos_equivalencias` ENABLE KEYS */;


--
-- Definition of table `db_version`
--

DROP TABLE IF EXISTS `db_version`;
CREATE TABLE `db_version` (
  `db_version` int(10) unsigned NOT NULL,
  PRIMARY KEY (`db_version`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `db_version`
--

/*!40000 ALTER TABLE `db_version` DISABLE KEYS */;
INSERT INTO `db_version` (`db_version`) VALUES 
 (0),
 (1),
 (2),
 (3),
 (4),
 (5),
 (6),
 (7),
 (8),
 (9),
 (10),
 (11),
 (12),
 (13),
 (14),
 (15),
 (16),
 (17),
 (18),
 (19),
 (20),
 (21),
 (22),
 (23),
 (24),
 (25),
 (26),
 (27),
 (28),
 (29);
/*!40000 ALTER TABLE `db_version` ENABLE KEYS */;


--
-- Definition of table `demandas`
--

DROP TABLE IF EXISTS `demandas`;
CREATE TABLE `demandas` (
  `dem_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` int(11) DEFAULT NULL,
  `dis_id` bigint(20) NOT NULL,
  `ofe_id` bigint(20) NOT NULL,
  `dem_quantidade` int(11) NOT NULL,
  PRIMARY KEY (`dem_id`),
  KEY `FK32558FBDEADC7214` (`dis_id`),
  KEY `FK32558FBD7A9BEDF9` (`ofe_id`),
  CONSTRAINT `FK32558FBD7A9BEDF9` FOREIGN KEY (`ofe_id`) REFERENCES `ofertas` (`ofe_id`),
  CONSTRAINT `FK32558FBDEADC7214` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB AUTO_INCREMENT=10787 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `demandas`
--

/*!40000 ALTER TABLE `demandas` DISABLE KEYS */;
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
-- Definition of table `dicas_eliminacao_prof_virtual`
--

DROP TABLE IF EXISTS `dicas_eliminacao_prof_virtual`;
CREATE TABLE `dicas_eliminacao_prof_virtual` (
  `dic_prv_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `dic_prv_dica_eliminacao` longtext NOT NULL,
  `version` int(11) DEFAULT NULL,
  `prf_id` bigint(20) NOT NULL,
  PRIMARY KEY (`dic_prv_id`),
  KEY `FKEBD44DB527898F49` (`prf_id`),
  CONSTRAINT `FKEBD44DB527898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `dicas_eliminacao_prof_virtual`
--

/*!40000 ALTER TABLE `dicas_eliminacao_prof_virtual` DISABLE KEYS */;
/*!40000 ALTER TABLE `dicas_eliminacao_prof_virtual` ENABLE KEYS */;


--
-- Definition of table `disciplinas`
--

DROP TABLE IF EXISTS `disciplinas`;
CREATE TABLE `disciplinas` (
  `dis_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `dis_codigo` varchar(20) NOT NULL,
  `dis_cred_pratico` int(11) NOT NULL,
  `dis_cred_teorico` int(11) NOT NULL,
  `dis_aulas_continuas` bit(1) NOT NULL DEFAULT b'0',
  `dis_professor_unico` bit(1) NOT NULL DEFAULT b'1',
  `dificuldade` int(11) NOT NULL,
  `dis_laboratorio` bit(1) DEFAULT NULL,
  `dis_max_alun_pratico` int(11) NOT NULL,
  `dis_max_alun_teorico` int(11) NOT NULL,
  `dis_nome` varchar(100) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `dcr_id` bigint(20) DEFAULT NULL,
  `tdi_id` bigint(20) NOT NULL,
  `dis_usa_sabado` bit(1) DEFAULT NULL,
  `dis_usa_domingo` bit(1) DEFAULT NULL,
  `dis_carga_horaria` int(11) DEFAULT NULL,
  PRIMARY KEY (`dis_id`),
  UNIQUE KEY `dis_codigo` (`dis_codigo`,`cen_id`),
  KEY `FK99F7D28767EDB695` (`cen_id`),
  KEY `FK99F7D287264F3B1D` (`tdi_id`),
  KEY `FK99F7D287A7B1F0B0` (`dcr_id`),
  CONSTRAINT `FK99F7D287264F3B1D` FOREIGN KEY (`tdi_id`) REFERENCES `tipos_disciplina` (`tdi_id`),
  CONSTRAINT `FK99F7D28767EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK99F7D287A7B1F0B0` FOREIGN KEY (`dcr_id`) REFERENCES `divisoes_credito` (`dcr_id`)
) ENGINE=InnoDB AUTO_INCREMENT=13056 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `disciplinas`
--

/*!40000 ALTER TABLE `disciplinas` DISABLE KEYS */;
/*!40000 ALTER TABLE `disciplinas` ENABLE KEYS */;


--
-- Definition of table `disciplinas_grupos_sala`
--

DROP TABLE IF EXISTS `disciplinas_grupos_sala`;
CREATE TABLE `disciplinas_grupos_sala` (
  `dis_id` bigint(20) NOT NULL,
  `grs_id` bigint(20) NOT NULL,
  KEY `FK_DIS_GRS_DIS_ID` (`dis_id`),
  KEY `FK_DIS_GRS_GRS_ID` (`grs_id`),
  CONSTRAINT `FK_DIS_GRS_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`),
  CONSTRAINT `FK_DIS_GRS_GRS_ID` FOREIGN KEY (`grs_id`) REFERENCES `grupos_sala` (`grs_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `disciplinas_grupos_sala`
--

/*!40000 ALTER TABLE `disciplinas_grupos_sala` DISABLE KEYS */;
/*!40000 ALTER TABLE `disciplinas_grupos_sala` ENABLE KEYS */;


--
-- Definition of table `disciplinas_professores_virtuais`
--

DROP TABLE IF EXISTS `disciplinas_professores_virtuais`;
CREATE TABLE `disciplinas_professores_virtuais` (
  `disciplinas` bigint(20) NOT NULL,
  `professores_virtuais` bigint(20) NOT NULL,
  PRIMARY KEY (`disciplinas`,`professores_virtuais`),
  KEY `FK3B075BC5D4324A2F` (`disciplinas`),
  KEY `FK3B075BC59208BDE5` (`professores_virtuais`),
  CONSTRAINT `FK3B075BC59208BDE5` FOREIGN KEY (`professores_virtuais`) REFERENCES `professores_virtuais` (`prf_id`),
  CONSTRAINT `FK3B075BC5D4324A2F` FOREIGN KEY (`disciplinas`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `disciplinas_professores_virtuais`
--

/*!40000 ALTER TABLE `disciplinas_professores_virtuais` DISABLE KEYS */;
/*!40000 ALTER TABLE `disciplinas_professores_virtuais` ENABLE KEYS */;


--
-- Definition of table `disciplinas_salas`
--

DROP TABLE IF EXISTS `disciplinas_salas`;
CREATE TABLE `disciplinas_salas` (
  `dis_id` bigint(20) NOT NULL,
  `sal_id` bigint(20) NOT NULL,
  KEY `FK_DIS_SAL_DIS_ID` (`dis_id`),
  KEY `FK_DIS_SAL_SAL_ID` (`sal_id`),
  CONSTRAINT `FK_DIS_SAL_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`),
  CONSTRAINT `FK_DIS_SAL_SAL_ID` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `disciplinas_salas`
--

/*!40000 ALTER TABLE `disciplinas_salas` DISABLE KEYS */;
/*!40000 ALTER TABLE `disciplinas_salas` ENABLE KEYS */;


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
  `ins_id` bigint(20) NOT NULL,
  PRIMARY KEY (`dcr_id`),
  KEY `FKB17F7F59A432FA9C` (`ins_id`),
  CONSTRAINT `FKB17F7F59A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`)
) ENGINE=InnoDB AUTO_INCREMENT=89 DEFAULT CHARSET=latin1;

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
  `dis_cursou_id` bigint(20) NOT NULL,
  `dis_elimina_id` bigint(20) NOT NULL,
  `eqv_geral` bit(1) DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`eqv_id`),
  KEY `FK_EQV_DIS_CURSOU_ID` (`dis_cursou_id`),
  KEY `FK_EQV_DIS_ELIMINA_ID` (`dis_elimina_id`),
  CONSTRAINT `FK_EQV_DIS_CURSOU_ID` FOREIGN KEY (`dis_cursou_id`) REFERENCES `disciplinas` (`dis_id`),
  CONSTRAINT `FK_EQV_DIS_ELIMINA_ID` FOREIGN KEY (`dis_elimina_id`) REFERENCES `disciplinas` (`dis_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `equivalencias`
--

/*!40000 ALTER TABLE `equivalencias` DISABLE KEYS */;
/*!40000 ALTER TABLE `equivalencias` ENABLE KEYS */;


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
  `ins_id` bigint(20) NOT NULL,
  PRIMARY KEY (`fix_id`),
  KEY `FKE8714F06AD343A9B` (`sal_id`),
  KEY `FKE8714F065121F2A6` (`cam_id`),
  KEY `FKE8714F0627898F49` (`prf_id`),
  KEY `FKE8714F06EADC7214` (`dis_id`),
  KEY `FKE8714F064E67EEBC` (`uni_id`),
  KEY `FKE8714F06A432FA9C` (`ins_id`),
  CONSTRAINT `FKE8714F0627898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`),
  CONSTRAINT `FKE8714F064E67EEBC` FOREIGN KEY (`uni_id`) REFERENCES `unidades` (`uni_id`),
  CONSTRAINT `FKE8714F065121F2A6` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`),
  CONSTRAINT `FKE8714F06A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
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
  `version` int(11) DEFAULT NULL,
  `hor_id` bigint(20) NOT NULL,
  `dia_semana` int(11) DEFAULT NULL,
  PRIMARY KEY (`hdc_id`),
  KEY `FK945A41B63E60F87E` (`hor_id`),
  CONSTRAINT `FK945A41B63E60F87E` FOREIGN KEY (`hor_id`) REFERENCES `horarios_aula` (`hor_id`)
) ENGINE=InnoDB AUTO_INCREMENT=1293 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horario_disponivel_cenario`
--

/*!40000 ALTER TABLE `horario_disponivel_cenario` DISABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=227 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `horarios_aula`
--

/*!40000 ALTER TABLE `horarios_aula` DISABLE KEYS */;
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
-- Definition of table `instituicao_ensino`
--

DROP TABLE IF EXISTS `instituicao_ensino`;
CREATE TABLE `instituicao_ensino` (
  `ins_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `ins_nome` varchar(255) NOT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`ins_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `instituicao_ensino`
--

/*!40000 ALTER TABLE `instituicao_ensino` DISABLE KEYS */;
INSERT INTO `instituicao_ensino` (`ins_id`,`ins_nome`,`version`) VALUES 
 (0,'Estácio',0);
/*!40000 ALTER TABLE `instituicao_ensino` ENABLE KEYS */;


--
-- Definition of table `motivos_uso_prof_virtual`
--

DROP TABLE IF EXISTS `motivos_uso_prof_virtual`;
CREATE TABLE `motivos_uso_prof_virtual` (
  `mot_prv_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `mot_prv_motivo_uso` longtext NOT NULL,
  `version` int(11) DEFAULT NULL,
  `prf_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`mot_prv_id`),
  KEY `FKFF95AAEF27898F49` (`prf_id`),
  CONSTRAINT `FKFF95AAEF27898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
) ENGINE=InnoDB AUTO_INCREMENT=406 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `motivos_uso_prof_virtual`
--

/*!40000 ALTER TABLE `motivos_uso_prof_virtual` DISABLE KEYS */;
/*!40000 ALTER TABLE `motivos_uso_prof_virtual` ENABLE KEYS */;


--
-- Definition of table `ofertas`
--

DROP TABLE IF EXISTS `ofertas`;
CREATE TABLE `ofertas` (
  `ofe_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` int(11) DEFAULT NULL,
  `cam_id` bigint(20) NOT NULL,
  `crc_id` bigint(20) NOT NULL,
  `tur_id` bigint(20) NOT NULL,
  `ofe_receita` double DEFAULT '0',
  `cur_id` bigint(20) NOT NULL,
  PRIMARY KEY (`ofe_id`),
  KEY `FKA3A6D8625121F2A6` (`cam_id`),
  KEY `FKA3A6D86256F127DF` (`tur_id`),
  KEY `FKA3A6D86274E10E16` (`crc_id`),
  KEY `FKA3A6D86238FF365A` (`cur_id`),
  CONSTRAINT `FKA3A6D86238FF365A` FOREIGN KEY (`cur_id`) REFERENCES `cursos` (`cur_id`),
  CONSTRAINT `FKA3A6D8625121F2A6` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`),
  CONSTRAINT `FKA3A6D86256F127DF` FOREIGN KEY (`tur_id`) REFERENCES `turnos` (`tur_id`),
  CONSTRAINT `FKA3A6D86274E10E16` FOREIGN KEY (`crc_id`) REFERENCES `curriculos` (`crc_id`)
) ENGINE=InnoDB AUTO_INCREMENT=404 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ofertas`
--

/*!40000 ALTER TABLE `ofertas` DISABLE KEYS */;
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
  `par_proibir_ciclos_equiv` bit(1) DEFAULT NULL,
  `par_considerar_transitividade_equiv` bit(1) DEFAULT NULL,
  `par_proibir_troca_por_disc_online_credzerados_equiv` bit(1) DEFAULT NULL,
  `par_evitarredcargahorprof` bit(1) DEFAULT NULL,
  `par_evitarredcargahorprofvalue` int(11) DEFAULT NULL,
  `par_evitarultimoprihorprof` bit(1) DEFAULT NULL,
  `par_funcaoobjetivo` int(11) DEFAULT NULL,
  `par_limmaxdiscpro` bit(1) DEFAULT NULL,
  `par_maxnotaavalcordoc` bit(1) DEFAULT NULL,
  `par_minalunturma` bit(1) DEFAULT NULL,
  `par_minalunturmavalue` int(11) DEFAULT NULL,
  `par_violar_min_turmas_formandos` bit(1) DEFAULT NULL,
  `par_mincustdoccur` bit(1) DEFAULT NULL,
  `par_mindeslaluno` bit(1) DEFAULT NULL,
  `par_mindeslprof` bit(1) DEFAULT NULL,
  `par_mindeslprofvalue` int(11) DEFAULT NULL,
  `par_mingapprof` bit(1) DEFAULT NULL,
  `par_modootimizacao` varchar(20) DEFAULT NULL,
  `par_niveldifdisci` bit(1) DEFAULT NULL,
  `par_otimizarpor` varchar(20) DEFAULT NULL,
  `par_percmindout` bit(1) DEFAULT NULL,
  `par_percminmest` bit(1) DEFAULT NULL,
  `par_prefprof` bit(1) DEFAULT NULL,
  `par_profmuitoscampi` bit(1) DEFAULT NULL,
  `par_regrasespdivcred` bit(1) DEFAULT NULL,
  `par_regrasgendivcre` bit(1) DEFAULT NULL,
  `par_utilizar_demandas_p2` bit(1) DEFAULT NULL,
  `par_capacidade_max_salas` bit(1) DEFAULT b'0',
  `par_evitarultimoprihorprofvalue` int(11) DEFAULT '11',
  `par_percminparcint` bit(1) DEFAULT b'0',
  `par_percminint` bit(1) DEFAULT b'0',
  `par_priorizarcalouros` bit(1) DEFAULT b'1',
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `ins_id` bigint(20) NOT NULL,
  `rop_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`par_id`),
  KEY `FK1B57F25A67EDB695` (`cen_id`),
  KEY `FK1B57F25AA432FA9C` (`ins_id`),
  KEY `FK1B57F25A9D41A9F6` (`rop_id`),
  CONSTRAINT `FK1B57F25A67EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK1B57F25A9D41A9F6` FOREIGN KEY (`rop_id`) REFERENCES `requisicao_otimizacao` (`rop_id`),
  CONSTRAINT `FK1B57F25AA432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`)
) ENGINE=InnoDB AUTO_INCREMENT=118 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parametros`
--

/*!40000 ALTER TABLE `parametros` DISABLE KEYS */;
/*!40000 ALTER TABLE `parametros` ENABLE KEYS */;


--
-- Definition of table `parametros_campi`
--

DROP TABLE IF EXISTS `parametros_campi`;
CREATE TABLE `parametros_campi` (
  `par_id` bigint(20) NOT NULL,
  `cam_id` bigint(20) NOT NULL,
  PRIMARY KEY (`par_id`,`cam_id`),
  KEY `FKE2C87FE3D74052F6` (`par_id`),
  KEY `FKE2C87FE35121F2A6` (`cam_id`),
  CONSTRAINT `FKE2C87FE35121F2A6` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`),
  CONSTRAINT `FKE2C87FE3D74052F6` FOREIGN KEY (`par_id`) REFERENCES `parametros` (`par_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parametros_campi`
--

/*!40000 ALTER TABLE `parametros_campi` DISABLE KEYS */;
/*!40000 ALTER TABLE `parametros_campi` ENABLE KEYS */;


--
-- Definition of table `parametros_configuracao`
--

DROP TABLE IF EXISTS `parametros_configuracao`;
CREATE TABLE `parametros_configuracao` (
  `cnf_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `cnf_url_oti` varchar(255) NOT NULL,
  `cnf_nome_oti` varchar(255) NOT NULL,
  `ins_id` bigint(20) NOT NULL,
  `version` int(11) DEFAULT NULL,
  PRIMARY KEY (`cnf_id`),
  KEY `FK_CNF_INS_ID` (`ins_id`),
  CONSTRAINT `FK_CNF_INS_ID` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parametros_configuracao`
--

/*!40000 ALTER TABLE `parametros_configuracao` DISABLE KEYS */;
/*!40000 ALTER TABLE `parametros_configuracao` ENABLE KEYS */;


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
-- Definition of table `parametros_turnos`
--

DROP TABLE IF EXISTS `parametros_turnos`;
CREATE TABLE `parametros_turnos` (
  `par_id` bigint(20) NOT NULL,
  `tur_id` bigint(20) NOT NULL,
  KEY `FK_PAR_TUR_PAR_ID` (`par_id`),
  KEY `FK_PAR_TUR_TUR_ID` (`tur_id`),
  CONSTRAINT `FK_PAR_TUR_PAR_ID` FOREIGN KEY (`par_id`) REFERENCES `parametros` (`par_id`),
  CONSTRAINT `FK_PAR_TUR_TUR_ID` FOREIGN KEY (`tur_id`) REFERENCES `turnos` (`tur_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parametros_turnos`
--

/*!40000 ALTER TABLE `parametros_turnos` DISABLE KEYS */;
/*!40000 ALTER TABLE `parametros_turnos` ENABLE KEYS */;


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
  `prf_nome` varchar(255) NOT NULL,
  `prf_valor_credito` double DEFAULT NULL,
  `prf_max_dias_semana` int(11) NOT NULL DEFAULT '5',
  `prf_min_cred_dia` int(11) NOT NULL DEFAULT '1',
  `version` int(11) DEFAULT NULL,
  `ati_id` bigint(20) DEFAULT NULL,
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
) ENGINE=InnoDB AUTO_INCREMENT=2188 DEFAULT CHARSET=latin1;

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
) ENGINE=InnoDB AUTO_INCREMENT=5660 DEFAULT CHARSET=latin1;

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
  `tit_id` bigint(20) DEFAULT NULL,
  `tco_id` bigint(20) DEFAULT NULL,
  `ins_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`prf_id`),
  KEY `FKD43F690DFA7DB545` (`tit_id`),
  KEY `FKD43F690DED717FE1` (`ati_id`),
  KEY `FKD43F690DA432FA9C` (`ins_id`),
  KEY `FK_CEN_ID_PRV` (`cen_id`),
  KEY `FK_TCO_ID_PRV` (`tco_id`),
  CONSTRAINT `FKD43F690DA432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FKD43F690DED717FE1` FOREIGN KEY (`ati_id`) REFERENCES `areas_titulacao` (`ati_id`),
  CONSTRAINT `FKD43F690DFA7DB545` FOREIGN KEY (`tit_id`) REFERENCES `titulacoes` (`tit_id`),
  CONSTRAINT `FK_CEN_ID_PRV` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK_TCO_ID_PRV` FOREIGN KEY (`tco_id`) REFERENCES `tipos_contrato` (`tco_id`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=latin1;

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
-- Definition of table `requisicao_otimizacao`
--

DROP TABLE IF EXISTS `requisicao_otimizacao`;
CREATE TABLE `requisicao_otimizacao` (
  `rop_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `rop_round` bigint(20) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `username` varchar(50) NOT NULL,
  `par_id` bigint(20) NOT NULL,
  PRIMARY KEY (`rop_id`),
  UNIQUE KEY `username` (`username`,`rop_round`,`cen_id`),
  KEY `FK28A7CC4079DB6948` (`username`),
  KEY `FK28A7CC4067EDB695` (`cen_id`),
  KEY `FK28A7CC40D74052F6` (`par_id`),
  CONSTRAINT `FK28A7CC4067EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FK28A7CC4079DB6948` FOREIGN KEY (`username`) REFERENCES `users` (`username`),
  CONSTRAINT `FK28A7CC40D74052F6` FOREIGN KEY (`par_id`) REFERENCES `parametros` (`par_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `requisicao_otimizacao`
--

/*!40000 ALTER TABLE `requisicao_otimizacao` DISABLE KEYS */;
/*!40000 ALTER TABLE `requisicao_otimizacao` ENABLE KEYS */;


--
-- Definition of table `salas`
--

DROP TABLE IF EXISTS `salas`;
CREATE TABLE `salas` (
  `sal_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `sal_andar` varchar(200) DEFAULT NULL,
  `sal_capacidade` int(11) NOT NULL,
  `sal_capacidade_max` int(11) NOT NULL DEFAULT '0',
  `sal_codigo` varchar(200) DEFAULT NULL,
  `sal_numero` varchar(200) DEFAULT NULL,
  `sal_descricao` varchar(255) DEFAULT NULL,
  `sal_custo_operacao_cred` double DEFAULT NULL,
  `version` int(11) DEFAULT NULL,
  `tsa_id` bigint(20) NOT NULL,
  `uni_id` bigint(20) NOT NULL,
  PRIMARY KEY (`sal_id`),
  UNIQUE KEY `uni_id` (`uni_id`,`sal_codigo`),
  KEY `FK6824890F84DC98B` (`tsa_id`),
  KEY `FK68248904E67EEBC` (`uni_id`),
  CONSTRAINT `FK68248904E67EEBC` FOREIGN KEY (`uni_id`) REFERENCES `unidades` (`uni_id`),
  CONSTRAINT `FK6824890F84DC98B` FOREIGN KEY (`tsa_id`) REFERENCES `tipos_sala` (`tsa_id`)
) ENGINE=InnoDB AUTO_INCREMENT=1891 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `salas`
--

/*!40000 ALTER TABLE `salas` DISABLE KEYS */;
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
  `ins_id` bigint(20) NOT NULL,
  `sle_tempo` int(11) NOT NULL,
  `cen_id` bigint(20) DEFAULT NULL,
  `sle_permite_intervalo_aula` bit(1) DEFAULT NULL,
  PRIMARY KEY (`sle_id`),
  KEY `FK9460D03FA432FA9C` (`ins_id`),
  KEY `FK_CEN_ID_SLE` (`cen_id`),
  KEY `sle_codigo` (`sle_codigo`,`cen_id`),
  CONSTRAINT `FK9460D03FA432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK_CEN_ID_SLE` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=40 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `semana_letiva`
--

/*!40000 ALTER TABLE `semana_letiva` DISABLE KEYS */;
/*!40000 ALTER TABLE `semana_letiva` ENABLE KEYS */;


--
-- Definition of table `tipos_contrato`
--

DROP TABLE IF EXISTS `tipos_contrato`;
CREATE TABLE `tipos_contrato` (
  `tco_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tco_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `ins_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`tco_id`),
  KEY `FKB9B6F79CA432FA9C` (`ins_id`),
  KEY `FK_CEN_ID_TCO` (`cen_id`),
  CONSTRAINT `FKB9B6F79CA432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK_CEN_ID_TCO` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tipos_contrato`
--

/*!40000 ALTER TABLE `tipos_contrato` DISABLE KEYS */;
INSERT INTO `tipos_contrato` (`tco_id`,`tco_nome`,`version`,`ins_id`,`cen_id`) VALUES 
 (1,'Horista',0,0,8),
 (2,'Tempo Parcial',0,0,8),
 (3,'Tempo Integral',0,0,8);
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
  `ins_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`tcu_id`),
  KEY `FKFD866C1CA432FA9C` (`ins_id`),
  KEY `FK_CEN_ID_TCU` (`cen_id`),
  CONSTRAINT `FKFD866C1CA432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK_CEN_ID_TCU` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tipos_curso`
--

/*!40000 ALTER TABLE `tipos_curso` DISABLE KEYS */;
/*!40000 ALTER TABLE `tipos_curso` ENABLE KEYS */;


--
-- Definition of table `tipos_disciplina`
--

DROP TABLE IF EXISTS `tipos_disciplina`;
CREATE TABLE `tipos_disciplina` (
  `tdi_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tdi_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `ins_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`tdi_id`),
  KEY `FK18165FECA432FA9C` (`ins_id`),
  KEY `FK_CEN_ID_TDI` (`cen_id`),
  CONSTRAINT `FK18165FECA432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK_CEN_ID_TDI` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tipos_disciplina`
--

/*!40000 ALTER TABLE `tipos_disciplina` DISABLE KEYS */;
INSERT INTO `tipos_disciplina` (`tdi_id`,`tdi_nome`,`version`,`ins_id`,`cen_id`) VALUES 
 (1,'Presencial',0,0,8),
 (2,'Online',0,0,8);
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
  `ins_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) DEFAULT NULL,
  `tsa_aceita_aula_pratica` bit(1) DEFAULT NULL,
  PRIMARY KEY (`tsa_id`),
  KEY `FKAD5DE4C3A432FA9C` (`ins_id`),
  KEY `FK_CEN_ID_TSA` (`cen_id`),
  CONSTRAINT `FKAD5DE4C3A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK_CEN_ID_TSA` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tipos_sala`
--

/*!40000 ALTER TABLE `tipos_sala` DISABLE KEYS */;
INSERT INTO `tipos_sala` (`tsa_id`,`tsa_descricao`,`tsa_nome`,`version`,`ins_id`,`cen_id`,`tsa_aceita_aula_pratica`) VALUES 
 (1,'Sala de Aula','Sala de Aula',0,0,8,0x00),
 (2,'Laboratório','Laboratório',0,0,8,0x01);
/*!40000 ALTER TABLE `tipos_sala` ENABLE KEYS */;


--
-- Definition of table `titulacoes`
--

DROP TABLE IF EXISTS `titulacoes`;
CREATE TABLE `titulacoes` (
  `tit_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tit_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `ins_id` bigint(20) NOT NULL,
  `cen_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`tit_id`),
  KEY `FK7E6B2C45A432FA9C` (`ins_id`),
  KEY `FK_CEN_ID_TIT` (`cen_id`),
  CONSTRAINT `FK7E6B2C45A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`),
  CONSTRAINT `FK_CEN_ID_TIT` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
) ENGINE=InnoDB AUTO_INCREMENT=46 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `titulacoes`
--

/*!40000 ALTER TABLE `titulacoes` DISABLE KEYS */;
INSERT INTO `titulacoes` (`tit_id`,`tit_nome`,`version`,`ins_id`,`cen_id`) VALUES 
 (1,'Licenciado',0,0,8),
 (2,'Bacharel',0,0,8),
 (3,'Especialista',0,0,8),
 (4,'Mestre',0,0,8),
 (5,'Doutor',0,0,8);
/*!40000 ALTER TABLE `titulacoes` ENABLE KEYS */;


--
-- Definition of table `turnos`
--

DROP TABLE IF EXISTS `turnos`;
CREATE TABLE `turnos` (
  `tur_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `tur_nome` varchar(50) NOT NULL,
  `version` int(11) DEFAULT NULL,
  `cen_id` bigint(20) NOT NULL,
  `ins_id` bigint(20) NOT NULL,
  PRIMARY KEY (`tur_id`),
  KEY `FKCC98632167EDB695` (`cen_id`),
  KEY `FKCC986321A432FA9C` (`ins_id`),
  CONSTRAINT `FKCC98632167EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
  CONSTRAINT `FKCC986321A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `turnos`
--

/*!40000 ALTER TABLE `turnos` DISABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=81 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `unidades`
--

/*!40000 ALTER TABLE `unidades` DISABLE KEYS */;
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
  `ins_id` bigint(20) NOT NULL,
  PRIMARY KEY (`username`),
  UNIQUE KEY `username` (`username`),
  KEY `FK6A68E0827898F49` (`prf_id`),
  KEY `FK6A68E08A432FA9C` (`ins_id`),
  CONSTRAINT `FK6A68E0827898F49` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`),
  CONSTRAINT `FK6A68E08A432FA9C` FOREIGN KEY (`ins_id`) REFERENCES `instituicao_ensino` (`ins_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--

/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` (`username`,`usu_email`,`enabled`,`usu_nome`,`password`,`version`,`prf_id`,`ins_id`) VALUES 
 ('admin','suporte@gapso.com.br',0x01,'Administrador','21232f297a57a5a743894a0e4a801fc3',1,NULL,0);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;


--
-- Definition of procedure `convertDBFrom_Version025_To_Version026`
--

DROP PROCEDURE IF EXISTS `convertDBFrom_Version025_To_Version026`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */ $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version025_To_Version026`()
BEGIN
  DECLARE status_code INT;
  DECLARE count_table_db_version INT;
  DECLARE actual_db_version INT;
  DECLARE msg_status VARCHAR(500);

  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    SET status_code = -1;
    ROLLBACK;
    SELECT status_code AS status_code, msg_status AS msg_status;
  END;

  SET status_code = 0;
  SET msg_status = 'Inicio da Transacao';

  
  START TRANSACTION;

    
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

    IF actual_db_version = 25 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      
      ALTER TABLE  `trieda`.`parametros` ADD COLUMN `par_priorizarcalouros` BIT(1) DEFAULT TRUE AFTER `par_percminint`;

      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (26);
      SET msg_status = 'Base de dados compativel com script de conversao, conversao realizada';
    ELSE
      SET msg_status = 'Base de dados nao compativel com script de conversao, conversao nao efetuada';
    END IF;
    
  
  IF (status_code = 0) THEN
    COMMIT;
  ELSE
    ROLLBACK;
  END IF;
    
  SELECT status_code AS status_code, msg_status AS msg_status;
END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `convertDBFrom_Version026_To_Version027`
--

DROP PROCEDURE IF EXISTS `convertDBFrom_Version026_To_Version027`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */ $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version026_To_Version027`()
BEGIN
  DECLARE status_code INT;
  DECLARE count_table_db_version INT;
  DECLARE actual_db_version INT;
  DECLARE msg_status VARCHAR(500);

  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    SET status_code = -1;
    ROLLBACK;
    SELECT status_code AS status_code, msg_status AS msg_status;
  END;

  SET status_code = 0;
  SET msg_status = 'Inicio da Transacao';

  
  START TRANSACTION;

    
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

    IF actual_db_version = 26 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      
		ALTER TABLE `trieda`.`demandas` DROP COLUMN `dem_quantidade`;


      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (27);
      SET msg_status = 'Base de dados compativel com script de conversao, conversao realizada';
    ELSE
      SET msg_status = 'Base de dados nao compativel com script de conversao, conversao nao efetuada';
    END IF;
    
  
  IF (status_code = 0) THEN
    COMMIT;
  ELSE
    ROLLBACK;
  END IF;
    
  SELECT status_code AS status_code, msg_status AS msg_status;
END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `convertDBFrom_Version027_To_Version028`
--

DROP PROCEDURE IF EXISTS `convertDBFrom_Version027_To_Version028`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */ $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version027_To_Version028`()
BEGIN
  DECLARE status_code INT;
  DECLARE count_table_db_version INT;
  DECLARE actual_db_version INT;
  DECLARE msg_status VARCHAR(500);

  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    SET status_code = -1;
    ROLLBACK;
    SELECT status_code AS status_code, msg_status AS msg_status;
  END;

  SET status_code = 0;
  SET msg_status = 'Inicio da Transacao';

  
  START TRANSACTION;

    
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

    IF actual_db_version = 27 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      
		ALTER TABLE `trieda`.`alunos_demanda` ADD COLUMN `ald_motivo_nao_atendimento` LONGTEXT;


      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (28);
      SET msg_status = 'Base de dados compativel com script de conversao, conversao realizada';
    ELSE
      SET msg_status = 'Base de dados nao compativel com script de conversao, conversao nao efetuada';
    END IF;
    
  
  IF (status_code = 0) THEN
    COMMIT;
  ELSE
    ROLLBACK;
  END IF;
    
  SELECT status_code AS status_code, msg_status AS msg_status;
END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `convertDBFrom_Version028_To_Version029`
--

DROP PROCEDURE IF EXISTS `convertDBFrom_Version028_To_Version029`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */ $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version028_To_Version029`()
BEGIN
  DECLARE status_code INT;
  DECLARE count_table_db_version INT;
  DECLARE actual_db_version INT;
  DECLARE msg_status VARCHAR(500);

  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    SET status_code = -1;
    ROLLBACK;
    SELECT status_code AS status_code, msg_status AS msg_status;
  END;

  SET status_code = 0;
  SET msg_status = 'Inicio da Transacao';

  
  START TRANSACTION;

    
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

    IF actual_db_version = 28 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      
      DROP TABLE IF EXISTS `trieda`.`motivos_uso_prof_virtual`;
      CREATE TABLE  `trieda`.`motivos_uso_prof_virtual` (
        `mot_prv_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `mot_prv_motivo_uso` LONGTEXT NOT NULL,
        `prf_id` bigint(20),
        `version` INTEGER,
        PRIMARY KEY (`mot_prv_id`),
        KEY `FK_MOT_PRF_ID` (`prf_id`),
        CONSTRAINT `FK_MOT_PRF_ID` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      DROP TABLE IF EXISTS `trieda`.`dicas_eliminacao_prof_virtual`;
      CREATE TABLE  `trieda`.`dicas_eliminacao_prof_virtual` (
        `dic_prv_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `dic_prv_dica_eliminacao` LONGTEXT NOT NULL,
        `prf_id` bigint(20) NOT NULL,
        `version` INTEGER,
        PRIMARY KEY (`dic_prv_id`),
        KEY `FK_DIC_PRF_ID` (`prf_id`),
        CONSTRAINT `FK_DIC_PRF_ID` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      DROP TABLE IF EXISTS `trieda`.`atendimento_operacional_motivos_uso`;
      CREATE TABLE  `trieda`.`atendimento_operacional_motivos_uso` (
        `atp_id` bigint(20) NOT NULL,
        `mot_prv_id` bigint(20) NOT NULL,
        KEY `FK_ATP_MOT_ATP_ID` (`atp_id`),
        KEY `FK_ATP_MOT_MOT_PRV_ID` (`mot_prv_id`),
        CONSTRAINT `FK_ATP_MOT_ATP_ID` FOREIGN KEY (`atp_id`) REFERENCES `atendimento_operacional` (`atp_id`),
        CONSTRAINT `FK_ATP_MOT_MOT_PRV_ID` FOREIGN KEY (`mot_prv_id`) REFERENCES `motivos_uso_prof_virtual` (`mot_prv_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      DROP TABLE IF EXISTS `trieda`.`atendimento_operacional_dicas_eliminacao`;
      CREATE TABLE  `trieda`.`atendimento_operacional_dicas_eliminacao` (
        `atp_id` bigint(20) NOT NULL,
        `dic_prv_id` bigint(20) NOT NULL,
        KEY `FK_ATP_DIC_ATP_ID` (`atp_id`),
        KEY `FK_ATP_DIC_DIC_PRV_ID` (`dic_prv_id`),
        CONSTRAINT `FK_ATP_DIC_ATP_ID` FOREIGN KEY (`atp_id`) REFERENCES `atendimento_operacional` (`atp_id`),
        CONSTRAINT `FK_ATP_DIC_DIC_PRV_ID` FOREIGN KEY (`dic_prv_id`) REFERENCES `dicas_eliminacao_prof_virtual` (`dic_prv_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      ALTER TABLE `trieda`.`professores_virtuais` ADD COLUMN `tco_id` BIGINT(20) AFTER `tit_id`,
        ADD CONSTRAINT `FK_TCO_ID_PRV` FOREIGN KEY `FK_TCO_ID_PRV` (`tco_id`) REFERENCES `tipos_contrato` (`tco_id`);

      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (29);
      SET msg_status = 'Base de dados compativel com script de conversao, conversao realizada';
    ELSE
      SET msg_status = 'Base de dados nao compativel com script de conversao, conversao nao efetuada';
    END IF;
    
  
  IF (status_code = 0) THEN
    COMMIT;
  ELSE
    ROLLBACK;
  END IF;
    
  SELECT status_code AS status_code, msg_status AS msg_status;
END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;



/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
