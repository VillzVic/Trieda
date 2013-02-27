DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version005_To_Version006` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version005_To_Version006`()
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

    IF actual_db_version = 5 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';
      
      /* TRIEDA-1529: Erro ao importar planilha de cursos onde há nomes de cursos com mais de 50 caracteres */
      ALTER TABLE `trieda`.`cursos` MODIFY COLUMN `cur_nome` VARCHAR(255);


      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (6);
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

CALL convertDBFrom_Version005_To_Version006();