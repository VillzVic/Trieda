drop index IDXPRFHORSEN on ALOCACAO;

drop table if exists ALOCACAO;

drop index IDXNOM on AREAS_DE_TITULACAO;

drop table if exists AREAS_DE_TITULACAO;

drop index IDXCOD on CALENDARIO;

drop table if exists CALENDARIO;

drop table if exists CAMPI;

drop index IDXUNI on CAMPUS_CURRICULO;

drop index IDXCRC on CAMPUS_CURRICULO;

drop index IDXTUR on CAMPUS_CURRICULO;

drop table if exists CAMPUS_CURRICULO;

drop index IDXNOMANOPER on CENARIOS;

drop table if exists CENARIOS;

drop index IDXDIS2 on COMPATIBILIDADE;

drop index IDXDIS1 on COMPATIBILIDADE;

drop table if exists COMPATIBILIDADE;

drop index IDXCUR on CURRICULO;

drop index IDXCOD on CURRICULO;

drop table if exists CURRICULO;

drop index IDXPER on CURRICULO_DISCIPLINA;

drop table if exists CURRICULO_DISCIPLINA;

drop index IDXTCU on CURSOS;

drop index IDXCUR on CURSOS;

drop index IDXCEN on CURSOS;

drop table if exists CURSOS;

drop index IDXATI on CURSO_AREA;

drop index IDXCUR on CURSO_AREA;

drop table if exists CURSO_AREA;

drop index IDXCURTURDIS on DEMANDA;

drop table if exists DEMANDA;

drop table if exists DESLOCAMENTO_CAMPI;

drop index IDXDESC on DESLOCAMENTO_UNIDADE;

drop index IDXORIG on DESLOCAMENTO_UNIDADE;

drop table if exists DESLOCAMENTO_UNIDADE;

drop index IDXCODNOMLAB on DISCIPLINAS;

drop index IDXCEN on DISCIPLINAS;

drop table if exists DISCIPLINAS;

drop index IDXDCR on DIVISAO_DE_CREDITOS;

drop table if exists DIVISAO_DE_CREDITOS;

drop index IDXELI on EQUIVALENCIA;

drop index IDXCURSOU on EQUIVALENCIA;

drop table if exists EQUIVALENCIA;

drop index IDXTUR on HORARIO_DE_AULA;

drop table if exists HORARIO_DE_AULA;

drop table if exists HORARIO_DISPONIVEL_CAMPUS;

drop index IDXHOR on HORARIO_DISP_CEN;

drop table if exists HORARIO_DISP_CEN;

drop index IDXDIS on HORARIO_DISP_DISC;

drop index IDXHDC on HORARIO_DISP_DISC;

drop table if exists HORARIO_DISP_DISC;

drop index IDXPRF on HORARIO_DISP_PROF;

drop index IDXHDC on HORARIO_DISP_PROF;

drop table if exists HORARIO_DISP_PROF;

drop index IDXHDC on HORARIO_DISP_SALA;

drop index IDXSAL on HORARIO_DISP_SALA;

drop table if exists HORARIO_DISP_SALA;

drop index IDXHDC on HORARIO_DISP_UNID;

drop index IDXUNI on HORARIO_DISP_UNID;

drop table if exists HORARIO_DISP_UNID;

drop index IDXTUMOFESAL on OFERECIMENTO;

drop table if exists OFERECIMENTO;

drop index INDEX_2 on PROFESSORES;

drop index INDEX_1 on PROFESSORES;

drop table if exists PROFESSORES;

drop index IDXPRF on PROFESSORES_CAMPUS;

drop index IDXUNI on PROFESSORES_CAMPUS;

drop table if exists PROFESSORES_CAMPUS;

drop index IDXPRF on PROFESSORES_DISCIPLINAS;

drop index IDXDIS on PROFESSORES_DISCIPLINAS;

drop table if exists PROFESSORES_DISCIPLINAS;

drop index IDXDCR on REGRAS_DE_CREDITOS;

drop index IDXCEN on REGRAS_DE_CREDITOS;

drop table if exists REGRAS_DE_CREDITOS;

drop index IDXCOD on SALAS;

drop index IDXUNI on SALAS;

drop table if exists SALAS;

drop index IDXDIS on SALA_DISCIPLINA;

drop index IDXSAL on SALA_DISCIPLINA;

drop table if exists SALA_DISCIPLINA;

drop index IDXUSU on SESSAO;

drop table if exists SESSAO;

drop index IDXNOM on TIPOS_CURSO;

drop table if exists TIPOS_CURSO;

drop index IDXNOM on TIPOS_SALA;

drop table if exists TIPOS_SALA;

drop index IDXNOM on TIPO_CONTRATO;

drop table if exists TIPO_CONTRATO;

drop index IDXNOM on TIPO_DE_DISCIPLINA;

drop table if exists TIPO_DE_DISCIPLINA;

drop index IDXNOM on TITULACOES;

drop table if exists TITULACOES;

drop index IDXDIS on TURMA;

drop index IDXNOM on TURMA;

drop table if exists TURMA;

drop index IDXCAL on TURNOS;

drop index IDXNOME on TURNOS;

drop table if exists TURNOS;

drop index IDXCODNOM on UNIDADES;

drop index IDXCEN on UNIDADES;

drop table if exists UNIDADES;

drop index IDXNOM on USUARIOS;

drop index IDXLOGSEN on USUARIOS;

drop table if exists USUARIOS;

/*==============================================================*/
/* Table: ALOCACAO                                              */
/*==============================================================*/
create table ALOCACAO
(
   OFE_ID               int(10) not null,
   PRF_ID               int(10) not null,
   HOR_ID               int(10) not null,
   HDC_ID               int(10) not null,
   primary key (OFE_ID, PRF_ID, HOR_ID, HDC_ID)
);

/*==============================================================*/
/* Index: IDXPRFHORSEN                                          */
/*==============================================================*/
create index IDXPRFHORSEN on ALOCACAO
(
   PRF_ID,
   HOR_ID
);

/*==============================================================*/
/* Table: AREAS_DE_TITULACAO                                    */
/*==============================================================*/
create table AREAS_DE_TITULACAO
(
   ATI_ID               int(10) not null auto_increment,
   ATI_NOME             varchar(255) not null,
   primary key (ATI_ID)
);

/*==============================================================*/
/* Index: IDXNOM                                                */
/*==============================================================*/
create index IDXNOM on AREAS_DE_TITULACAO
(
   ATI_NOME
);

/*==============================================================*/
/* Table: CALENDARIO                                            */
/*==============================================================*/
create table CALENDARIO
(
   CAL_ID               int(10) not null auto_increment,
   CEN_ID               int(10) not null,
   CAL_CODIGO           varchar(255) not null,
   CAL_DESCRICAO        varchar(255),
   CAL_TEMPO_AULA       int(2) not null,
   primary key (CAL_ID)
);

/*==============================================================*/
/* Index: IDXCOD                                                */
/*==============================================================*/
create index IDXCOD on CALENDARIO
(
   CAL_CODIGO
);

/*==============================================================*/
/* Table: CAMPI                                                 */
/*==============================================================*/
create table CAMPI
(
   CAM_ID               int(10) not null auto_increment,
   CEN_ID               int(10),
   CAM_CODIGO           varchar(255),
   CAM_NOME             varchar(255),
   primary key (CAM_ID)
);

/*==============================================================*/
/* Table: CAMPUS_CURRICULO                                      */
/*==============================================================*/
create table CAMPUS_CURRICULO
(
   TUR_ID               int(10) not null,
   CRC_ID               int(10) not null,
   CAM_ID               int(10) not null,
   primary key (TUR_ID, CRC_ID, CAM_ID)
);

/*==============================================================*/
/* Index: IDXTUR                                                */
/*==============================================================*/
create index IDXTUR on CAMPUS_CURRICULO
(
   TUR_ID
);

/*==============================================================*/
/* Index: IDXCRC                                                */
/*==============================================================*/
create index IDXCRC on CAMPUS_CURRICULO
(
   CRC_ID
);

/*==============================================================*/
/* Index: IDXUNI                                                */
/*==============================================================*/
create index IDXUNI on CAMPUS_CURRICULO
(
   
);

/*==============================================================*/
/* Table: CENARIOS                                              */
/*==============================================================*/
create table CENARIOS
(
   CEN_ID               int(10) not null auto_increment,
   USU_CRIADO_ID        int(10),
   USU_ATUALIZADO_ID    int(10),
   CEN_NOME             varchar(255) not null,
   CEN_ANO              int(4) not null,
   CEN_PERIODO          int(2) not null,
   CEN_DT_CRIACAO       datetime not null,
   CEN_DT_ATUALIZACAO   datetime,
   CEN_COMENTARIO       text,
   CEN_OFICIAL          boolean not null,
   primary key (CEN_ID)
);

/*==============================================================*/
/* Index: IDXNOMANOPER                                          */
/*==============================================================*/
create index IDXNOMANOPER on CENARIOS
(
   CEN_NOME,
   CEN_ANO,
   CEN_PERIODO
);

/*==============================================================*/
/* Table: COMPATIBILIDADE                                       */
/*==============================================================*/
create table COMPATIBILIDADE
(
   DIS1_ID              int(10) not null,
   DIS2_ID              int(10) not null,
   primary key (DIS1_ID, DIS2_ID)
);

/*==============================================================*/
/* Index: IDXDIS1                                               */
/*==============================================================*/
create index IDXDIS1 on COMPATIBILIDADE
(
   DIS1_ID
);

/*==============================================================*/
/* Index: IDXDIS2                                               */
/*==============================================================*/
create index IDXDIS2 on COMPATIBILIDADE
(
   DIS2_ID
);

/*==============================================================*/
/* Table: CURRICULO                                             */
/*==============================================================*/
create table CURRICULO
(
   CRC_ID               int(10) not null auto_increment,
   CUR_ID               int(10) not null,
   CEN_ID               int(10) not null,
   CRC_CODIGO           varchar(255) not null,
   CRC_DESCRICAO        varchar(255),
   primary key (CRC_ID)
);

/*==============================================================*/
/* Index: IDXCOD                                                */
/*==============================================================*/
create index IDXCOD on CURRICULO
(
   CRC_CODIGO
);

/*==============================================================*/
/* Index: IDXCUR                                                */
/*==============================================================*/
create index IDXCUR on CURRICULO
(
   CUR_ID
);

/*==============================================================*/
/* Table: CURRICULO_DISCIPLINA                                  */
/*==============================================================*/
create table CURRICULO_DISCIPLINA
(
   CRC_ID               int(10) not null,
   DIS_ID               int(10) not null,
   CDI_PERIODO          int(2),
   primary key (CRC_ID, DIS_ID)
);

/*==============================================================*/
/* Index: IDXPER                                                */
/*==============================================================*/
create index IDXPER on CURRICULO_DISCIPLINA
(
   CDI_PERIODO
);

/*==============================================================*/
/* Table: CURSOS                                                */
/*==============================================================*/
create table CURSOS
(
   CUR_ID               int(10) not null auto_increment,
   CEN_ID               int(10) not null,
   TCU_ID               int(10),
   CUR_CODIGO           varchar(255) not null,
   CUR_NUM_PERIODOS     int(2),
   CUR_MIN_DOUTORES     int(3),
   CUR_MIN_MESTRES      int(3),
   CUR_MAX_PROF_DISCIPINA int(2),
   CUR_ADM_MAIS_DE_UMA_DISC boolean,
   primary key (CUR_ID)
);

/*==============================================================*/
/* Index: IDXCEN                                                */
/*==============================================================*/
create index IDXCEN on CURSOS
(
   CEN_ID
);

/*==============================================================*/
/* Index: IDXCUR                                                */
/*==============================================================*/
create index IDXCUR on CURSOS
(
   CUR_CODIGO
);

/*==============================================================*/
/* Index: IDXTCU                                                */
/*==============================================================*/
create index IDXTCU on CURSOS
(
   TCU_ID
);

/*==============================================================*/
/* Table: CURSO_AREA                                            */
/*==============================================================*/
create table CURSO_AREA
(
   CUR_ID               int(10) not null,
   ATI_ID               int(10) not null,
   primary key (CUR_ID, ATI_ID)
);

/*==============================================================*/
/* Index: IDXCUR                                                */
/*==============================================================*/
create index IDXCUR on CURSO_AREA
(
   CUR_ID
);

/*==============================================================*/
/* Index: IDXATI                                                */
/*==============================================================*/
create index IDXATI on CURSO_AREA
(
   ATI_ID
);

/*==============================================================*/
/* Table: DEMANDA                                               */
/*==============================================================*/
create table DEMANDA
(
   UNI_ID               int(10) not null,
   CUR_ID               int(10) not null,
   TUR_ID               int(10) not null,
   DIS_ID               int(10) not null,
   DEM_QUANTIDADE       int(4),
   primary key (UNI_ID, CUR_ID, TUR_ID, DIS_ID)
);

/*==============================================================*/
/* Index: IDXCURTURDIS                                          */
/*==============================================================*/
create index IDXCURTURDIS on DEMANDA
(
   CUR_ID,
   TUR_ID,
   DIS_ID
);

/*==============================================================*/
/* Table: DESLOCAMENTO_CAMPI                                    */
/*==============================================================*/
create table DESLOCAMENTO_CAMPI
(
   CAM_DEST_ID          int(10),
   CAM_ORIG_ID          int(10),
   DEC_TEMPO            int(3) not null
);

/*==============================================================*/
/* Table: DESLOCAMENTO_UNIDADE                                  */
/*==============================================================*/
create table DESLOCAMENTO_UNIDADE
(
   UNI_ORIG_ID          int(10) not null,
   UNI_DEST_ID          int(10) not null,
   DEU_TEMPO            int(3) not null,
   primary key (UNI_ORIG_ID, UNI_DEST_ID)
);

/*==============================================================*/
/* Index: IDXORIG                                               */
/*==============================================================*/
create index IDXORIG on DESLOCAMENTO_UNIDADE
(
   UNI_ORIG_ID
);

/*==============================================================*/
/* Index: IDXDESC                                               */
/*==============================================================*/
create index IDXDESC on DESLOCAMENTO_UNIDADE
(
   UNI_DEST_ID
);

/*==============================================================*/
/* Table: DISCIPLINAS                                           */
/*==============================================================*/
create table DISCIPLINAS
(
   DIS_ID               int(10) not null auto_increment,
   TDI_ID               int(10) not null,
   DCR_ID               int(10),
   CEN_ID               int(10) not null,
   DIS_CODIGO           varchar(255) not null,
   DIS_NOME             varchar(255) not null,
   DIS_CRED_TEORICOS    int(2) not null,
   DIS_CRED_PRATICOS    int(2) not null,
   DIS_LABORATORIO      boolean not null,
   DIS_TIPO             varchar(255) not null,
   DIS_DIFICULDADE      int(1),
   DIS_MAX_ALUN_TEORICOS int(2),
   DIS_MAX_ALUN_PRATICOS int(2),
   primary key (DIS_ID)
);

/*==============================================================*/
/* Index: IDXCEN                                                */
/*==============================================================*/
create index IDXCEN on DISCIPLINAS
(
   CEN_ID
);

/*==============================================================*/
/* Index: IDXCODNOMLAB                                          */
/*==============================================================*/
create index IDXCODNOMLAB on DISCIPLINAS
(
   DIS_CODIGO,
   DIS_NOME,
   DIS_LABORATORIO
);

/*==============================================================*/
/* Table: DIVISAO_DE_CREDITOS                                   */
/*==============================================================*/
create table DIVISAO_DE_CREDITOS
(
   DCR_ID               int(10) not null auto_increment,
   DCR_CREDITOS         int(2) not null,
   DCR_DIA1             int(2) not null,
   DCR_DIA2             int(2) not null,
   DCR_DIA3             int(2) not null,
   DCR_DIA4             int(2) not null,
   DCR_DIA5             int(2) not null,
   DCR_DIA6             int(2) not null,
   DCR_DIA7             int(2) not null,
   primary key (DCR_ID)
);

/*==============================================================*/
/* Index: IDXDCR                                                */
/*==============================================================*/
create index IDXDCR on DIVISAO_DE_CREDITOS
(
   DCR_CREDITOS
);

/*==============================================================*/
/* Table: EQUIVALENCIA                                          */
/*==============================================================*/
create table EQUIVALENCIA
(
   DIS_CURSOU_ID        int(10) not null,
   DIS_ELIMINA_ID       int(10) not null,
   primary key (DIS_CURSOU_ID, DIS_ELIMINA_ID)
);

/*==============================================================*/
/* Index: IDXCURSOU                                             */
/*==============================================================*/
create index IDXCURSOU on EQUIVALENCIA
(
   DIS_CURSOU_ID
);

/*==============================================================*/
/* Index: IDXELI                                                */
/*==============================================================*/
create index IDXELI on EQUIVALENCIA
(
   DIS_ELIMINA_ID
);

/*==============================================================*/
/* Table: HORARIO_DE_AULA                                       */
/*==============================================================*/
create table HORARIO_DE_AULA
(
   HOR_ID               int(10) not null auto_increment,
   TUR_ID               int(10) not null,
   HOR_INICIO           time not null,
   primary key (HOR_ID)
);

/*==============================================================*/
/* Index: IDXTUR                                                */
/*==============================================================*/
create index IDXTUR on HORARIO_DE_AULA
(
   TUR_ID
);

/*==============================================================*/
/* Table: HORARIO_DISPONIVEL_CAMPUS                             */
/*==============================================================*/
create table HORARIO_DISPONIVEL_CAMPUS
(
   HDC_ID               int(10),
   CAM_ID               int(10)
);

/*==============================================================*/
/* Table: HORARIO_DISP_CEN                                      */
/*==============================================================*/
create table HORARIO_DISP_CEN
(
   HDC_ID               int(10) not null auto_increment,
   HOR_ID               int(10) not null,
   HDC_SEMANA           int(1) not null,
   primary key (HDC_ID)
);

/*==============================================================*/
/* Index: IDXHOR                                                */
/*==============================================================*/
create index IDXHOR on HORARIO_DISP_CEN
(
   HOR_ID
);

/*==============================================================*/
/* Table: HORARIO_DISP_DISC                                     */
/*==============================================================*/
create table HORARIO_DISP_DISC
(
   HDC_ID               int(10) not null,
   DIS_ID               int(10) not null,
   primary key (HDC_ID, DIS_ID)
);

/*==============================================================*/
/* Index: IDXHDC                                                */
/*==============================================================*/
create index IDXHDC on HORARIO_DISP_DISC
(
   HDC_ID
);

/*==============================================================*/
/* Index: IDXDIS                                                */
/*==============================================================*/
create index IDXDIS on HORARIO_DISP_DISC
(
   DIS_ID
);

/*==============================================================*/
/* Table: HORARIO_DISP_PROF                                     */
/*==============================================================*/
create table HORARIO_DISP_PROF
(
   HDC_ID               int(10) not null,
   PRF_ID               int(10) not null,
   primary key (HDC_ID, PRF_ID)
);

/*==============================================================*/
/* Index: IDXHDC                                                */
/*==============================================================*/
create index IDXHDC on HORARIO_DISP_PROF
(
   HDC_ID
);

/*==============================================================*/
/* Index: IDXPRF                                                */
/*==============================================================*/
create index IDXPRF on HORARIO_DISP_PROF
(
   PRF_ID
);

/*==============================================================*/
/* Table: HORARIO_DISP_SALA                                     */
/*==============================================================*/
create table HORARIO_DISP_SALA
(
   HDC_ID               int(10) not null,
   SAL_ID               int(10) not null,
   primary key (HDC_ID, SAL_ID)
);

/*==============================================================*/
/* Index: IDXSAL                                                */
/*==============================================================*/
create index IDXSAL on HORARIO_DISP_SALA
(
   SAL_ID
);

/*==============================================================*/
/* Index: IDXHDC                                                */
/*==============================================================*/
create index IDXHDC on HORARIO_DISP_SALA
(
   HDC_ID
);

/*==============================================================*/
/* Table: HORARIO_DISP_UNID                                     */
/*==============================================================*/
create table HORARIO_DISP_UNID
(
   HDC_ID               int(10) not null,
   UNI_ID               int(10) not null,
   primary key (UNI_ID, HDC_ID)
);

/*==============================================================*/
/* Index: IDXUNI                                                */
/*==============================================================*/
create index IDXUNI on HORARIO_DISP_UNID
(
   UNI_ID
);

/*==============================================================*/
/* Index: IDXHDC                                                */
/*==============================================================*/
create index IDXHDC on HORARIO_DISP_UNID
(
   HDC_ID
);

/*==============================================================*/
/* Table: OFERECIMENTO                                          */
/*==============================================================*/
create table OFERECIMENTO
(
   OFE_ID               int(10) not null auto_increment,
   TUM_ID               int(10) not null,
   SAL_ID               int(10) not null,
   OFE_SEMANA           int(1) not null,
   OFE_CREDITOS         int(1) not null,
   primary key (OFE_ID)
);

/*==============================================================*/
/* Index: IDXTUMOFESAL                                          */
/*==============================================================*/
create index IDXTUMOFESAL on OFERECIMENTO
(
   TUM_ID,
   SAL_ID,
   OFE_SEMANA
);

/*==============================================================*/
/* Table: PROFESSORES                                           */
/*==============================================================*/
create table PROFESSORES
(
   PRF_ID               int(10) not null auto_increment,
   CEN_ID               int(10) not null,
   TCO_ID               int(10) not null,
   TIT_ID               int(10) not null,
   ATI_ID               int(10) not null,
   PRF_CPF              varchar(14) not null,
   PRF_NOME             varchar(255) not null,
   PRF_CH_MIN           int(3),
   PRF_CH_MAX           int(3),
   CRED_ANTERIOR        varchar(3),
   PRF_VALOR_CREDITO    double(7,2),
   PRF_NOTA             int(2),
   primary key (PRF_ID)
);

/*==============================================================*/
/* Index: INDEX_1                                               */
/*==============================================================*/
create index INDEX_1 on PROFESSORES
(
   CEN_ID
);

/*==============================================================*/
/* Index: INDEX_2                                               */
/*==============================================================*/
create index INDEX_2 on PROFESSORES
(
   PRF_CPF,
   PRF_NOME
);

/*==============================================================*/
/* Table: PROFESSORES_CAMPUS                                    */
/*==============================================================*/
create table PROFESSORES_CAMPUS
(
   PRF_ID               int(10) not null,
   CAM_ID               int(10) not null,
   primary key (PRF_ID, CAM_ID)
);

/*==============================================================*/
/* Index: IDXUNI                                                */
/*==============================================================*/
create index IDXUNI on PROFESSORES_CAMPUS
(
   
);

/*==============================================================*/
/* Index: IDXPRF                                                */
/*==============================================================*/
create index IDXPRF on PROFESSORES_CAMPUS
(
   PRF_ID
);

/*==============================================================*/
/* Table: PROFESSORES_DISCIPLINAS                               */
/*==============================================================*/
create table PROFESSORES_DISCIPLINAS
(
   DIS_ID               int(10) not null,
   PRF_ID               int(10) not null,
   PFD_PREFERENCIA      int(2) not null,
   PFD_NOTA             int(2) not null,
   primary key (DIS_ID, PRF_ID)
);

/*==============================================================*/
/* Index: IDXDIS                                                */
/*==============================================================*/
create index IDXDIS on PROFESSORES_DISCIPLINAS
(
   DIS_ID
);

/*==============================================================*/
/* Index: IDXPRF                                                */
/*==============================================================*/
create index IDXPRF on PROFESSORES_DISCIPLINAS
(
   PRF_ID
);

/*==============================================================*/
/* Table: REGRAS_DE_CREDITOS                                    */
/*==============================================================*/
create table REGRAS_DE_CREDITOS
(
   CEN_ID               int(10) not null,
   DCR_ID               int(10) not null,
   primary key (CEN_ID, DCR_ID)
);

/*==============================================================*/
/* Index: IDXCEN                                                */
/*==============================================================*/
create index IDXCEN on REGRAS_DE_CREDITOS
(
   CEN_ID
);

/*==============================================================*/
/* Index: IDXDCR                                                */
/*==============================================================*/
create index IDXDCR on REGRAS_DE_CREDITOS
(
   DCR_ID
);

/*==============================================================*/
/* Table: SALAS                                                 */
/*==============================================================*/
create table SALAS
(
   SAL_ID               int(10) not null auto_increment,
   TSA_ID               int(10) not null,
   UNI_ID               int(10) not null,
   SAL_CODIGO           varchar(255) not null,
   SAL_NUM              varchar(255) not null,
   SAL_ANDAR            varchar(255) not null,
   SAL_CAPACIDADE       int(3) not null,
   primary key (SAL_ID)
);

/*==============================================================*/
/* Index: IDXUNI                                                */
/*==============================================================*/
create index IDXUNI on SALAS
(
   UNI_ID
);

/*==============================================================*/
/* Index: IDXCOD                                                */
/*==============================================================*/
create index IDXCOD on SALAS
(
   SAL_CODIGO
);

/*==============================================================*/
/* Table: SALA_DISCIPLINA                                       */
/*==============================================================*/
create table SALA_DISCIPLINA
(
   SAL_ID               int(10) not null,
   DIS_ID               int(10) not null,
   primary key (SAL_ID, DIS_ID)
);

/*==============================================================*/
/* Index: IDXSAL                                                */
/*==============================================================*/
create index IDXSAL on SALA_DISCIPLINA
(
   SAL_ID
);

/*==============================================================*/
/* Index: IDXDIS                                                */
/*==============================================================*/
create index IDXDIS on SALA_DISCIPLINA
(
   DIS_ID
);

/*==============================================================*/
/* Table: SESSAO                                                */
/*==============================================================*/
create table SESSAO
(
   SES_ID               int(10) not null auto_increment,
   USU_ID               int(10) not null,
   SES_HORARIO          datetime not null,
   primary key (SES_ID)
);

/*==============================================================*/
/* Index: IDXUSU                                                */
/*==============================================================*/
create index IDXUSU on SESSAO
(
   USU_ID
);

/*==============================================================*/
/* Table: TIPOS_CURSO                                           */
/*==============================================================*/
create table TIPOS_CURSO
(
   TCU_ID               int(10) not null auto_increment,
   TCU_NOME             varchar(255) not null,
   primary key (TCU_ID)
);

/*==============================================================*/
/* Index: IDXNOM                                                */
/*==============================================================*/
create index IDXNOM on TIPOS_CURSO
(
   TCU_NOME
);

/*==============================================================*/
/* Table: TIPOS_SALA                                            */
/*==============================================================*/
create table TIPOS_SALA
(
   TSA_ID               int(10) not null,
   TSA_NOME             varchar(255) not null,
   TSA_DESCRICAO        varchar(255),
   primary key (TSA_ID)
);

/*==============================================================*/
/* Index: IDXNOM                                                */
/*==============================================================*/
create index IDXNOM on TIPOS_SALA
(
   TSA_NOME
);

/*==============================================================*/
/* Table: TIPO_CONTRATO                                         */
/*==============================================================*/
create table TIPO_CONTRATO
(
   TCO_ID               int(10) not null,
   TCO_NOME             varchar(255) not null,
   primary key (TCO_ID)
);

/*==============================================================*/
/* Index: IDXNOM                                                */
/*==============================================================*/
create index IDXNOM on TIPO_CONTRATO
(
   TCO_NOME
);

/*==============================================================*/
/* Table: TIPO_DE_DISCIPLINA                                    */
/*==============================================================*/
create table TIPO_DE_DISCIPLINA
(
   TDI_ID               int(10) not null,
   TDI_NOME             varchar(255) not null,
   primary key (TDI_ID)
);

/*==============================================================*/
/* Index: IDXNOM                                                */
/*==============================================================*/
create index IDXNOM on TIPO_DE_DISCIPLINA
(
   TDI_NOME
);

/*==============================================================*/
/* Table: TITULACOES                                            */
/*==============================================================*/
create table TITULACOES
(
   TIT_ID               int(10) not null,
   TIT_NOME             varchar(255) not null,
   primary key (TIT_ID)
);

/*==============================================================*/
/* Index: IDXNOM                                                */
/*==============================================================*/
create index IDXNOM on TITULACOES
(
   TIT_NOME
);

/*==============================================================*/
/* Table: TURMA                                                 */
/*==============================================================*/
create table TURMA
(
   TUM_ID               int(10) not null auto_increment,
   DIS_ID               int(10) not null,
   TUM_NOME             varchar(255) not null,
   primary key (TUM_ID)
);

/*==============================================================*/
/* Index: IDXNOM                                                */
/*==============================================================*/
create index IDXNOM on TURMA
(
   TUM_NOME
);

/*==============================================================*/
/* Index: IDXDIS                                                */
/*==============================================================*/
create index IDXDIS on TURMA
(
   DIS_ID
);

/*==============================================================*/
/* Table: TURNOS                                                */
/*==============================================================*/
create table TURNOS
(
   TUR_ID               int(10) not null auto_increment,
   CAL_ID               int(10) not null,
   TUR_NOME             varchar(255) not null,
   primary key (TUR_ID)
);

/*==============================================================*/
/* Index: IDXNOME                                               */
/*==============================================================*/
create index IDXNOME on TURNOS
(
   TUR_NOME
);

/*==============================================================*/
/* Index: IDXCAL                                                */
/*==============================================================*/
create index IDXCAL on TURNOS
(
   CAL_ID
);

/*==============================================================*/
/* Table: UNIDADES                                              */
/*==============================================================*/
create table UNIDADES
(
   UNI_ID               int(10) not null auto_increment,
   UNI_CODIGO           varchar(255) not null,
   UNI_NOME             varchar(255) not null,
   UNI_ENDERECO         varchar(255),
   UNI_NUM_MED_SALAS    int(3),
   UNI_CUST_MED_CRED    double(7,2),
   primary key (UNI_ID)
);

/*==============================================================*/
/* Index: IDXCEN                                                */
/*==============================================================*/
create index IDXCEN on UNIDADES
(
   
);

/*==============================================================*/
/* Index: IDXCODNOM                                             */
/*==============================================================*/
create index IDXCODNOM on UNIDADES
(
   UNI_CODIGO,
   UNI_NOME
);

/*==============================================================*/
/* Table: USUARIOS                                              */
/*==============================================================*/
create table USUARIOS
(
   USU_ID               int(10) not null auto_increment,
   USU_NOME             varchar(255) not null,
   USU_EMAIL            varchar(255) not null,
   USU_LOGIN            varchar(255) not null,
   USU_SENHA            varchar(255) not null,
   primary key (USU_ID)
);

/*==============================================================*/
/* Index: IDXLOGSEN                                             */
/*==============================================================*/
create index IDXLOGSEN on USUARIOS
(
   USU_LOGIN,
   USU_SENHA
);

/*==============================================================*/
/* Index: IDXNOM                                                */
/*==============================================================*/
create index IDXNOM on USUARIOS
(
   USU_NOME
);

alter table ALOCACAO add constraint FK_HDC_ALO foreign key (HDC_ID)
      references HORARIO_DISP_CEN (HDC_ID) on delete restrict on update restrict;

alter table ALOCACAO add constraint FK_OFE_ALO foreign key (OFE_ID)
      references OFERECIMENTO (OFE_ID) on delete cascade on update cascade;

alter table ALOCACAO add constraint FK_PFR_ALO foreign key (PRF_ID)
      references PROFESSORES (PRF_ID) on delete restrict on update restrict;

alter table CALENDARIO add constraint FK_CEN_CAL foreign key (CEN_ID)
      references CENARIOS (CEN_ID) on delete cascade on update cascade;

alter table CAMPI add constraint FK_CEN_CAM foreign key (CEN_ID)
      references CENARIOS (CEN_ID) on delete cascade on update restrict;

alter table CAMPUS_CURRICULO add constraint FK_CAM_CAC foreign key (CAM_ID)
      references CAMPI (CAM_ID) on delete cascade on update restrict;

alter table CAMPUS_CURRICULO add constraint FK_CRC_UNC foreign key (CRC_ID)
      references CURRICULO (CRC_ID) on delete cascade on update cascade;

alter table CAMPUS_CURRICULO add constraint FK_TUR_UNC foreign key (TUR_ID)
      references TURNOS (TUR_ID) on delete cascade on update cascade;

alter table CENARIOS add constraint FK_USU_CEN1 foreign key (USU_ATUALIZADO_ID)
      references USUARIOS (USU_ID) on delete set null on update set null;

alter table CENARIOS add constraint FK_USU_CEN2 foreign key (USU_CRIADO_ID)
      references USUARIOS (USU_ID) on delete set null on update set null;

alter table COMPATIBILIDADE add constraint FK_DIS_COM1 foreign key (DIS1_ID)
      references DISCIPLINAS (DIS_ID) on delete cascade on update cascade;

alter table COMPATIBILIDADE add constraint FK_DIS_COM2 foreign key (DIS2_ID)
      references DISCIPLINAS (DIS_ID) on delete cascade on update cascade;

alter table CURRICULO add constraint FK_CEN_CUR foreign key (CEN_ID)
      references CENARIOS (CEN_ID) on delete cascade on update cascade;

alter table CURRICULO add constraint FK_CUR_CRC foreign key (CUR_ID)
      references CURSOS (CUR_ID) on delete cascade on update cascade;

alter table CURRICULO_DISCIPLINA add constraint FK_CRC_CRD foreign key (CRC_ID)
      references CURRICULO (CRC_ID) on delete cascade on update cascade;

alter table CURRICULO_DISCIPLINA add constraint FK_DIS_CRD foreign key (DIS_ID)
      references DISCIPLINAS (DIS_ID) on delete restrict on update restrict;

alter table CURSOS add constraint FK_CEN_CUR foreign key (CEN_ID)
      references CENARIOS (CEN_ID) on delete cascade on update cascade;

alter table CURSOS add constraint FK_TCU_CUR foreign key (TCU_ID)
      references TIPOS_CURSO (TCU_ID) on delete restrict on update restrict;

alter table CURSO_AREA add constraint FK_ATI_CUA foreign key (ATI_ID)
      references AREAS_DE_TITULACAO (ATI_ID) on delete restrict on update restrict;

alter table CURSO_AREA add constraint FK_CUR_CUA foreign key (CUR_ID)
      references CURSOS (CUR_ID) on delete cascade on update cascade;

alter table DEMANDA add constraint FK_CUR_DEM foreign key (CUR_ID)
      references CURSOS (CUR_ID) on delete cascade on update cascade;

alter table DEMANDA add constraint FK_DIC_DEM foreign key (DIS_ID)
      references DISCIPLINAS (DIS_ID) on delete cascade on update cascade;

alter table DEMANDA add constraint FK_TUR_DEM foreign key (TUR_ID)
      references TURNOS (TUR_ID) on delete cascade on update cascade;

alter table DEMANDA add constraint FK_UNI_DEM foreign key (UNI_ID)
      references UNIDADES (UNI_ID) on delete cascade on update cascade;

alter table DESLOCAMENTO_CAMPI add constraint FK_CAM_DEC1 foreign key (CAM_DEST_ID)
      references CAMPI (CAM_ID) on delete cascade on update restrict;

alter table DESLOCAMENTO_CAMPI add constraint FK_CAM_DEC2 foreign key (CAM_ORIG_ID)
      references CAMPI (CAM_ID) on delete cascade on update restrict;

alter table DESLOCAMENTO_UNIDADE add constraint FK_UNI_DES1 foreign key (UNI_ORIG_ID)
      references UNIDADES (UNI_ID) on delete cascade on update cascade;

alter table DESLOCAMENTO_UNIDADE add constraint FK_UNI_DES2 foreign key (UNI_DEST_ID)
      references UNIDADES (UNI_ID) on delete cascade on update cascade;

alter table DISCIPLINAS add constraint FK_CEN_DIS foreign key (CEN_ID)
      references CENARIOS (CEN_ID) on delete cascade on update cascade;

alter table DISCIPLINAS add constraint FK_DCR_DIC foreign key (DCR_ID)
      references DIVISAO_DE_CREDITOS (DCR_ID) on delete set null on update set null;

alter table DISCIPLINAS add constraint FK_TDI_DIS foreign key (TDI_ID)
      references TIPO_DE_DISCIPLINA (TDI_ID) on delete restrict on update restrict;

alter table EQUIVALENCIA add constraint FK_DIS_EQV1 foreign key (DIS_CURSOU_ID)
      references DISCIPLINAS (DIS_ID) on delete cascade on update cascade;

alter table EQUIVALENCIA add constraint FK_DIS_EQV2 foreign key (DIS_ELIMINA_ID)
      references DISCIPLINAS (DIS_ID) on delete cascade on update cascade;

alter table HORARIO_DE_AULA add constraint FK_TUR_HOR foreign key (TUR_ID)
      references TURNOS (TUR_ID) on delete cascade on update cascade;

alter table HORARIO_DISPONIVEL_CAMPUS add constraint FK_CAM_HDA foreign key (CAM_ID)
      references CAMPI (CAM_ID) on delete cascade on update restrict;

alter table HORARIO_DISPONIVEL_CAMPUS add constraint FK_HDC_HDA foreign key (HDC_ID)
      references HORARIO_DISP_CEN (HDC_ID) on delete cascade on update restrict;

alter table HORARIO_DISP_CEN add constraint FK_HOR_HDC foreign key (HOR_ID)
      references HORARIO_DE_AULA (HOR_ID) on delete cascade on update cascade;

alter table HORARIO_DISP_DISC add constraint FK_DIS_HDD foreign key (DIS_ID)
      references DISCIPLINAS (DIS_ID) on delete cascade on update cascade;

alter table HORARIO_DISP_DISC add constraint FK_HOR_HDD foreign key (HDC_ID)
      references HORARIO_DISP_CEN (HDC_ID) on delete cascade on update cascade;

alter table HORARIO_DISP_PROF add constraint FK_HOR_HDP foreign key (HDC_ID)
      references HORARIO_DISP_CEN (HDC_ID) on delete cascade on update cascade;

alter table HORARIO_DISP_PROF add constraint FK_PRF_HDP foreign key (PRF_ID)
      references PROFESSORES (PRF_ID) on delete cascade on update cascade;

alter table HORARIO_DISP_SALA add constraint FK_HOR_HDS foreign key (HDC_ID)
      references HORARIO_DISP_CEN (HDC_ID) on delete cascade on update cascade;

alter table HORARIO_DISP_SALA add constraint FK_SAL_HDS foreign key (SAL_ID)
      references SALAS (SAL_ID) on delete cascade on update cascade;

alter table HORARIO_DISP_UNID add constraint FK_HOR_HDU foreign key (HDC_ID)
      references HORARIO_DISP_CEN (HDC_ID) on delete cascade on update cascade;

alter table HORARIO_DISP_UNID add constraint FK_UNI_HDU foreign key (UNI_ID)
      references UNIDADES (UNI_ID) on delete cascade on update cascade;

alter table OFERECIMENTO add constraint FK_SAL_OFE foreign key (SAL_ID)
      references SALAS (SAL_ID) on delete restrict on update restrict;

alter table OFERECIMENTO add constraint FK_TUR_OFE foreign key (TUM_ID)
      references TURMA (TUM_ID) on delete cascade on update cascade;

alter table PROFESSORES add constraint FK_ATI_PRF foreign key (ATI_ID)
      references AREAS_DE_TITULACAO (ATI_ID) on delete restrict on update restrict;

alter table PROFESSORES add constraint FK_CEN_PRF foreign key (CEN_ID)
      references CENARIOS (CEN_ID) on delete cascade on update cascade;

alter table PROFESSORES add constraint FK_TCO_PRF foreign key (TCO_ID)
      references TIPO_CONTRATO (TCO_ID) on delete restrict on update restrict;

alter table PROFESSORES add constraint FK_TIT_PRF foreign key (TIT_ID)
      references TITULACOES (TIT_ID) on delete restrict on update restrict;

alter table PROFESSORES_CAMPUS add constraint FK_CAM_PRC foreign key (CAM_ID)
      references CAMPI (CAM_ID) on delete cascade on update restrict;

alter table PROFESSORES_CAMPUS add constraint FK_PRF_PFU foreign key (PRF_ID)
      references PROFESSORES (PRF_ID) on delete cascade on update cascade;

alter table PROFESSORES_DISCIPLINAS add constraint FK_DIS_PFD foreign key (DIS_ID)
      references DISCIPLINAS (DIS_ID) on delete cascade on update cascade;

alter table PROFESSORES_DISCIPLINAS add constraint FK_PRF_PFD foreign key (PRF_ID)
      references PROFESSORES (PRF_ID) on delete cascade on update cascade;

alter table REGRAS_DE_CREDITOS add constraint FK_CEN_RCR foreign key (CEN_ID)
      references CENARIOS (CEN_ID) on delete cascade on update cascade;

alter table REGRAS_DE_CREDITOS add constraint FK_DCR_RCR foreign key (DCR_ID)
      references DIVISAO_DE_CREDITOS (DCR_ID) on delete cascade on update cascade;

alter table SALAS add constraint FK_TSA_SAL foreign key (TSA_ID)
      references TIPOS_SALA (TSA_ID) on delete restrict on update restrict;

alter table SALAS add constraint FK_UNI_SAL foreign key (UNI_ID)
      references UNIDADES (UNI_ID) on delete cascade on update cascade;

alter table SALA_DISCIPLINA add constraint FK_DIS_SAD foreign key (DIS_ID)
      references DISCIPLINAS (DIS_ID) on delete cascade on update cascade;

alter table SALA_DISCIPLINA add constraint FK_SAL_SAD foreign key (SAL_ID)
      references SALAS (SAL_ID) on delete cascade on update cascade;

alter table SESSAO add constraint FK_USU_SES foreign key (USU_ID)
      references USUARIOS (USU_ID) on delete cascade on update cascade;

alter table TURMA add constraint FK_DIS_TUR foreign key (DIS_ID)
      references DISCIPLINAS (DIS_ID) on delete restrict on update restrict;

alter table TURNOS add constraint FK_CAL_TUR foreign key (CAL_ID)
      references CALENDARIO (CAL_ID) on delete cascade on update cascade;
