#drop database ets_2_12;
#create database ets_2_12;
#use ets_2_12;

drop index HELP_FILE_ALL on HELP_FILE ;

drop index HELP_FILE_ENTITY on HELP_FILE;

drop index IX1_CHANNEL_LIST on CHANNEL_LIST;

drop index IX1_CHANNEL_LIST_TO_CHANNEL on CHANNEL_LIST_TO_CHANNEL;

drop index IX1_DUTY_CYCLE_CONDITION on DUTY_CYCLE_CONDITION;

drop index IX1_MASK_FEATURE on MASK_FEATURE;

drop index IX1_MASK_TO_MASK_FEATURE on MASK_TO_MASK_FEATURE;

drop index IX1_MEDIUM_CHANNEL on MEDIUM_CHANNEL;

drop index IX1_PRODUCT_ATTR on PRODUCT_ATTR;

drop index IX1_PROGRAM_PLUGIN on PROGRAM_PLUGIN;

drop index IX1_PROGRAM_TO_MASK_FEATURE on PROGRAM_TO_MASK_FEATURE;

drop index TEXT_ATTRIBUTE_ENTITY on TEXT_ATTRIBUTE;

drop index TEXT_ATTRIBUTE_I1 on TEXT_ATTRIBUTE;

drop index device_accessory_device_id on DEVICE_ACCESSORY;

drop index device_object_device_id on DEVICE_OBJECT;

drop index device_parameter_device_id on DEVICE_PARAMETER;

drop index maingroup_project_id on MAINGROUP;

drop table VD_TO_DIST;

drop table LINE_FILTER_TABLE;

drop table DEVICE_TO_TENDER;

drop table SUBGROUP_TO_OBJECT;

drop table DEVICE_EXTDATA;

drop table DEVICE_PROGRAMMING;

drop table DEVICE_TO_SPECIFICATION;

drop table SUBGROUP_TO_LINE2;

drop table DEVICE_TO_POLLING_GROUP;

drop table DEVICE;

drop table DEVICE_PARAMETER;

drop table POLLING_GROUP;

drop table DEVICE_ACCESSORY;

drop table LINE;

drop table DUTY_CYCLE_CONDITION;

drop table EIB_PROPERTY;

drop table TEXT_TO_CATENT;

drop table PRODUCT_DESCRIPTION;

drop table EIB_OBJECT;

drop table DEVICE_OBJECT;

drop table ACCESSORY;

drop table COMMUNICATION_OBJECT;

drop table PARAMETER_LIST_OF_VALUES;

drop table VIRTUAL_DEVICE;

drop table S19_BLOCK_PARAGRAPH;

drop table SYMBOL_CONDITION;

drop table CATALOG_ENTRY;

drop table PARAMETER;

drop table CALCULATION;

drop table PROJECT_HISTORY;

drop table PROGRAM_PLUGIN;

drop table PROGRAM_DESCRIPTION;

drop table PRODUCT_TO_PROGRAM_TO_MT;

drop table PARAMETER_TYPE;

drop table EFFORT_TO_TENDER;

drop table ADDRESS_FIXUP;

drop table PRODUCT_TO_PROGRAM;

drop table MASK_TO_MASK_FEATURE;

drop table DEVICE_INFO_VALUE;

drop table TENDER;

drop table PROGRAM_TO_MASK_FEATURE;

drop table DEVICE_INFO;

drop table S19_BLOCK;

drop table APPLICATION_PROGRAM;

drop table ROOM;

drop table MASK_ENTRY;

drop table CHANNEL_LIST_TO_CHANNEL;

drop table MASK_SYSTEM_POINTER;

drop table PRODUCT_ATTR;

drop table AREA;

drop table MASK_COMPATIBILITY;

drop table FUNCTION;

drop table ADDITIONAL_FILE;

drop table SYSID_SCAN;

drop table SYSTEM_ID_LOOKUP;

drop table MASK;

drop table MANUFACTURER_COMPATIBILITY;

drop table PROJECT_CONFIG;

drop table PROJECT_CHECK;

drop table PRODUCT_CATALOG;

drop table SUBGROUP;

drop table SPECIFICATION_TEXT;

drop table BUILDING;

drop table CHANNEL_LIST;

drop table PROJECT;

drop table EIB_DATA_SUBTYPE;

drop table HELP_FILE;

drop table HW_PRODUCT;

drop table MIDDLEGROUP;

drop table PERSON;

drop table DISTRIBUTION_LIST;

drop table TEXT_ATTRIBUTE;

drop table FUNCTIONAL_ENTITY;

drop table MANUFACTURER;

drop table MEDIUM_CHANNEL;

drop table ADDRESS;

drop table SCHEMA_COLUMNS_H;

drop table SCHEMA_COLUMNS;

drop table S19_CONTROL_CODE;

drop table SPECIFICATION;

drop table SCHEMA_TABLES_H;

drop table ADDRESS_FORMAT;

drop table OBJECT_TYPE;

drop table CURRENCY;

drop table MEDIUM_TYPE;

drop table SCHEMA_TABLES;

drop table SCHEMA_PROVIDER_TYPES;

drop table BCU_TYPE;

drop table ASSEMBLER;

drop table MASK_FEATURE;

drop table SYMBOL;

drop table ETE_LANGUAGE;

drop table ETS_DATABASE_VERSION;

drop table _PASSWORD;

drop table ADDITIONAL_EFFORT;

drop table PEI_TYPE;

drop table IMEX_WHERE_CLAUSE;

drop table COMPONENT_TYPE;

drop table ADDITIONAL_TEXT;

drop table EIB_DATA_TYPE;

drop table PARAMETER_ATOMIC_TYPE;

drop table MAINGROUP;

drop table IMEX_BINARY;

drop table IMEX_TABLE_ORDER;

drop table IMEX_FS;

drop table ID_ALLOCATION;

drop table IMEX_OMIT;

drop table IMEX_FILES;

drop table IMEX_KEYS;

drop table IMEX_PK;

drop table OBJECT_PRIORITY;

drop table QUANTITY_UNIT;

drop table EAGLE_INFO;

drop view Accessories;

drop view AddressFixups;

drop view ApplicationPrograms;

drop view Areas;

drop view AssociationTemplates;

drop view Associations;

drop view BCUTypes;

drop view Buildings;

drop view CatalogEntries;

drop view CommunicationObjects;

drop view _Database;

drop view DeviceAccessories;

drop view DeviceCommunicationObjects;

drop view DeviceExtDatas;

drop view DeviceParameters;

drop view DevicePollingGroups;

drop view DeviceTemplateAccessories;

drop view DeviceTemplateCommObjects;

drop view DeviceTemplateParameters;

drop view DeviceTemplates;

drop view Devices;

drop view DistributionLists;

drop view DownloadParameters;

drop view EibDataSubtypes;

drop view EibDataTypes;

drop view FunctionalEntities;

drop view Functions;

drop view GroupAddressTemplates;

drop view GroupAddresses;

drop view HardwareProducts;

drop view Languages;

drop view _Lines;

drop view MainGroupTemplates;

drop view MainGroups;

drop view ManufacturerCompatibilities;

drop view Manufacturers;

drop view MaskCompatibilities;

drop view MaskEntries;

drop view MaskFeatures;

drop view MaskToMaskFeatures;

drop view Masks;

drop view MediumChannels;

drop view MediumTypes;

drop view MiddleGroupTemplates;

drop view MiddleGroups;

drop view ParameterEnumValues;

drop view ParameterTypes;

drop view Parameters;

drop view PollingGroups;

drop view ProductDescriptions;

drop view ProductProgramMediumTypes;

drop view ProductPrograms;

drop view ProgramDescriptions;

drop view ProgramPlugins;

drop view ProgramToMaskFeatures;

drop view ProjectHistories;

drop view ProjectParts;

drop view Projects;

drop view Rooms;

drop view Symbols;

drop view TextAttributes;

drop view Translations;

drop view UIParameters;

drop view maingroup_view;

drop view middlegroup_view;

#drop view prefab_finder;

#drop view product_finder;

#drop view product_finder_old;

drop view quantity_structure;

#drop view regtool_finder;

#drop view subgroup_to_line;

create table EAGLE_INFO (
INFO_NAME            VARCHAR(128)                   not null,
INFO_VALUE           LONG VARCHAR,
primary key (INFO_NAME)
) ENGINE = INNODB;

create table QUANTITY_UNIT (
QUANTITY_UNIT_NUMBER SMALLINT                       not null,
QUANTITY_UNIT_NAME   VARCHAR(50),
primary key (QUANTITY_UNIT_NUMBER)
) ENGINE = INNODB;

create table OBJECT_PRIORITY (
OBJECT_PRIORITY_CODE SMALLINT                       not null,
OBJECT_PRIORITY_NAME VARCHAR(50),
primary key (OBJECT_PRIORITY_CODE)
) ENGINE = INNODB;

create table IMEX_PK (
IMEX_PK_ID           NUMERIC(6)                     not null,
PK_TABLE_NAME        VARCHAR(35),
PK_COLUMN_NAME       VARCHAR(35),
primary key (IMEX_PK_ID)
) ENGINE = INNODB;

create table IMEX_KEYS (
IMEX_KEYS_ID         NUMERIC(6)                     not null,
KEYS_TABLE_NAME      VARCHAR(35),
KEYS_COLUMN_NAME     VARCHAR(35),
primary key (IMEX_KEYS_ID)
) ENGINE = INNODB;

create table IMEX_FILES (
IMEX_FILES_ID        NUMERIC(6)                     not null,
FILES_TABLE          VARCHAR(35),
FILES_COLUMN         VARCHAR(35),
primary key (IMEX_FILES_ID)
) ENGINE = INNODB;

create table IMEX_OMIT (
IMEX_OMIT_ID         NUMERIC(6)                     not null,
MAJOR_DB_VERSION_NUMBER SMALLINT,
MINOR_DB_VERSION_NUMBER SMALLINT,
TABLE_NAME           VARCHAR(35),
COLUMN_NAME          VARCHAR(35),
primary key (IMEX_OMIT_ID)
) ENGINE = INNODB;

create table ID_ALLOCATION (
ID_COUNTER           INTEGER                        not null,
primary key (ID_COUNTER)
) ENGINE = INNODB;

create table IMEX_FS (
IMEX_FS_ID           NUMERIC(6)                     not null,
GOUP                 SMALLINT,
FATHER_TABLE_NAME    VARCHAR(35),
SON_TABLE_NAME       VARCHAR(35),
PRIMARY_KEY_COLUMN   VARCHAR(35),
FOREIGN_KEY_COLUMN   VARCHAR(35),
WHERE_CLAUSE         VARCHAR(255),
ROOT_TABLE_NAME      VARCHAR(35),
primary key (IMEX_FS_ID)
) ENGINE = INNODB;

create table IMEX_TABLE_ORDER (
ORDER_TABLE          VARCHAR(35)                    not null,
TABLE_NUMBER         SMALLINT                       not null,
ROOT_TABLE           VARCHAR(35)                    not null,
primary key (ORDER_TABLE, ROOT_TABLE)
) ENGINE = INNODB;

create table IMEX_BINARY (
BINARY_TABLE_NAME    VARCHAR(35)                    not null,
BINARY_COLUMN_NAME   VARCHAR(35)                    not null,
primary key (BINARY_TABLE_NAME, BINARY_COLUMN_NAME)
) ENGINE = INNODB;

create table MAINGROUP (
PROJECT_ID           INTEGER,
MAINGROUP_ID         INTEGER                        not null,
MAINGROUP_NAME       VARCHAR(50),
MAINGROUP_ADDRESS    INTEGER,
MAINGROUP_PASSTHROUGH SMALLINT,
MAINGROUP_DESCRIPTION VARCHAR(80),
MAINGROUP_LOCKED     INTEGER,
MAINGROUP_UNIQUE_NAME VARCHAR(50),
LAST_MIDDLEGROUP_ADDRESS SMALLINT,
primary key (MAINGROUP_ID)
) ENGINE = INNODB;

create table PARAMETER_ATOMIC_TYPE (
ATOMIC_TYPE_NUMBER   SMALLINT                       not null,
ATOMIC_TYPE_NAME     VARCHAR(50),
DISPATTR             VARCHAR(1),
primary key (ATOMIC_TYPE_NUMBER)
) ENGINE = INNODB;

create table EIB_DATA_TYPE (
EIB_DATA_TYPE_CODE   SMALLINT                       not null,
EIB_DATA_TYPE_NAME   VARCHAR(50),
LENGTH_IN_BIT        SMALLINT,
EIS_TYPE_NAME        VARCHAR(50),
primary key (EIB_DATA_TYPE_CODE)
) ENGINE = INNODB;

create table ADDITIONAL_TEXT (
ADDITIONAL_TEXT_ID   INTEGER                        not null,
ADDITIONAL_TEXT_NAME VARCHAR(50),
ADDITIONAL_TEXT_NUMB VARCHAR(20),
ADDITIONAL_TEXT_FILENAME VARCHAR(64),
primary key (ADDITIONAL_TEXT_ID)
) ENGINE = INNODB;

create table COMPONENT_TYPE (
COMPONENT_TYPE_NUMBER SMALLINT                       not null,
COMPONENT_TYPE_NAME  VARCHAR(50),
COMPONENT_TYPE_ATTRIBUTES SMALLINT,
primary key (COMPONENT_TYPE_NUMBER)
) ENGINE = INNODB;

create table IMEX_WHERE_CLAUSE (
IMEX_WHERE_CLAUSE_ID NUMERIC(6)                     not null,
ROOT_TABLE_NAME      VARCHAR(35),
TABLE_NAME           VARCHAR(35),
WHERE_CLAUSE         VARCHAR(255),
primary key (IMEX_WHERE_CLAUSE_ID)
) ENGINE = INNODB;

create table PEI_TYPE (
PEI_TYPE_NUMBER      SMALLINT                       not null,
PEI_TYPE_NAME        VARCHAR(50),
PEI_TYPE_MIN         SMALLINT,
PEI_TYPE_MID         SMALLINT,
PEI_TYPE_MAX         SMALLINT,
primary key (PEI_TYPE_NUMBER)
) ENGINE = INNODB;

create table ADDITIONAL_EFFORT (
EFFORT_ID            INTEGER                        not null,
EFFORT_NAME          VARCHAR(50),
EFFORT_TYPE          VARCHAR(1),
EFFORT_WAGE          NUMERIC(12,3),
EFFORT_CURRENCY      VARCHAR(3),
primary key (EFFORT_ID)
) ENGINE = INNODB;

create table _PASSWORD (
MODULE_NAME          VARCHAR(50)                    not null,
MODULE_PASSWORD      VARCHAR(20),
primary key (MODULE_NAME)
) ENGINE = INNODB;

create table ETS_DATABASE_VERSION (
MAJOR_VERSION_NUMBER SMALLINT                       not null,
MINOR_VERSION_NUMBER SMALLINT                       not null,
USER_TYPE            SMALLINT,
DATABASE_ID          VARCHAR(38),
primary key (MAJOR_VERSION_NUMBER, MINOR_VERSION_NUMBER)
) ENGINE = INNODB;

create table ETE_LANGUAGE (
LANGUAGE_ID          INTEGER                        not null,
LANGUAGE_NAME        VARCHAR(50),
DATABASE_LANGUAGE    SMALLINT,
primary key (LANGUAGE_ID)
) ENGINE = INNODB;

create table SYMBOL (
SYMBOL_ID            INTEGER                        not null,
SYMBOL_FILENAME      VARCHAR(80),
SYMBOL_NAME          VARCHAR(50),
SYMBOL_DATA          LONG VARBINARY,
primary key (SYMBOL_ID)
) ENGINE = INNODB;

create table MASK_FEATURE (
MASK_FEATURE_ID      INTEGER                        not null,
MASK_FEATURE_NAME    VARCHAR(20)                    not null,
primary key (MASK_FEATURE_ID)
) ENGINE = INNODB;

create table ASSEMBLER (
ASSEMBLER_ID         INTEGER                        not null,
ASSEMBLER_NAME       VARCHAR(50),
ASSEMBLER_TYPE       SMALLINT,
primary key (ASSEMBLER_ID)
) ENGINE = INNODB;

create table BCU_TYPE (
BCU_TYPE_NUMBER      SMALLINT                       not null,
BCU_TYPE_NAME        VARCHAR(50),
BCU_TYPE_CPU         VARCHAR(50),
primary key (BCU_TYPE_NUMBER)
) ENGINE = INNODB;

create table SCHEMA_PROVIDER_TYPES (
TYPE_NAME            VARCHAR(128)                   not null,
DATA_TYPE            INTEGER                        not null,
COLUMN_SIZE          INTEGER                        not null,
LITERAL_PREFIX       VARCHAR(128),
LITERAL_SUFFIX       VARCHAR(128),
CREATE_PARAMS        VARCHAR(128),
IS_NULLABLE          INTEGER                        not null,
CASE_SENSITIVE       INTEGER                        not null,
SEARCHABLE           INTEGER                        not null,
UNSIGNED_ATTRIBUTE   INTEGER,
FIXED_PREC_SCALE     INTEGER                        not null,
AUTO_UNIQUE_VALUE    INTEGER                        not null,
LOCAL_TYPE_NAME      VARCHAR(128)                   not null,
MINIMUM_SCALE        INTEGER,
MAXIMUM_SCALE        INTEGER,
GUID                 VARCHAR(128),
TYPELIB              VARCHAR(128),
VERSION              VARCHAR(128),
IS_LONG              INTEGER                        not null,
BEST_MATCH           INTEGER,
IS_FIXEDLENGTH       INTEGER                        not null,
primary key (TYPE_NAME)
) ENGINE = INNODB;

create table SCHEMA_TABLES (
TABLE_CATALOG        VARCHAR(128)                   not null,
TABLE_SCHEMA         VARCHAR(128)                   not null,
TABLE_NAME           VARCHAR(128)                   not null,
TABLE_TYPE           VARCHAR(128),
TABLE_GUID           VARCHAR(128),
DESCRIPTION          VARCHAR(128),
TABLE_PROPID         INTEGER,
primary key (TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME)
) ENGINE = INNODB;

create table MEDIUM_TYPE (
MEDIUM_TYPE_NUMBER   SMALLINT                       not null,
MEDIUM_TYPE_NAME     VARCHAR(50),
MEDIUM_TYPE_SHORT_NAME VARCHAR(20),
STANDARD_SYSTEM_ID_LEVEL VARCHAR(1),
SYSTEM_ID_LENGTH     SMALLINT,
DCY_F_INDIVIDUAL     FLOAT,
DCY_G_INDIVIDUAL     FLOAT,
DCY_F_GROUP          FLOAT,
DCY_G_GROUP          FLOAT,
DUTY_CYCLE_CALCULATION SMALLINT,
primary key (MEDIUM_TYPE_NUMBER)
) ENGINE = INNODB;

create table CURRENCY (
CURRENCY_NUMBER      SMALLINT                       not null,
CURRENCY_NAME        VARCHAR(50),
primary key (CURRENCY_NUMBER)
) ENGINE = INNODB;

create table OBJECT_TYPE (
OBJECT_TYPE_CODE     SMALLINT                       not null,
OBJECT_TYPE_NAME     VARCHAR(50),
LENGTH_IN_BIT        SMALLINT,
primary key (OBJECT_TYPE_CODE)
) ENGINE = INNODB;

create table ADDRESS_FORMAT (
ADDRESS_FORMAT_ID    INTEGER                        not null,
ADDRESS_FORMAT_NAME  VARCHAR(50),
DISPLAY_TEXT_1       VARCHAR(20),
DISPLAY_TEXT_2       VARCHAR(20),
DISPLAY_TEXT_3       VARCHAR(20),
DISPLAY_TEXT_4       VARCHAR(20),
DISPLAY_TEXT_5       VARCHAR(20),
DISPLAY_TEXT_6       VARCHAR(20),
DISPLAY_TEXT_7       VARCHAR(20),
DISPLAY_TEXT_8       VARCHAR(20),
DISPLAY_TEXT_9       VARCHAR(20),
DISPLAY_TEXT_10      VARCHAR(20),
DISPLAY_TEXT_11      VARCHAR(20),
DISPLAY_TEXT_12      VARCHAR(20),
DISPLAY_TEXT_13      VARCHAR(20),
DISPLAY_TEXT_14      VARCHAR(20),
DISPLAY_TEXT_15      VARCHAR(20),
KEY_COLUMN           SMALLINT,
primary key (ADDRESS_FORMAT_ID)
) ENGINE = INNODB;

create table SCHEMA_TABLES_H (
TABLE_CATALOG        VARCHAR(128)                   not null,
TABLE_SCHEMA         VARCHAR(128)                   not null,
TABLE_NAME           VARCHAR(128)                   not null,
TABLE_TYPE           VARCHAR(128),
TABLE_GUID           VARCHAR(128),
DESCRIPTION          VARCHAR(128),
TABLE_PROPID         INTEGER,
primary key (TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME)
) ENGINE = INNODB;

create table SPECIFICATION (
SPECIFICATION_ID     INTEGER                        not null,
SPECIFICATION_NAME   VARCHAR(50),
SPECIFICATION_NUMB   VARCHAR(20),
SPECIFICATION_DATE   DATE,
ADDITIONAL_TEXT_ID   INTEGER,
ADD_ADDITIONAL_TEXT_ID INTEGER,
ADD2_ADDITIONAL_TEXT_ID INTEGER,
ADD3_ADDITIONAL_TEXT_ID INTEGER,
primary key (SPECIFICATION_ID),
foreign key (ADDITIONAL_TEXT_ID)
      references ADDITIONAL_TEXT (ADDITIONAL_TEXT_ID),
foreign key (ADD_ADDITIONAL_TEXT_ID)
      references ADDITIONAL_TEXT (ADDITIONAL_TEXT_ID),
foreign key (ADD2_ADDITIONAL_TEXT_ID)
      references ADDITIONAL_TEXT (ADDITIONAL_TEXT_ID),
foreign key (ADD3_ADDITIONAL_TEXT_ID)
      references ADDITIONAL_TEXT (ADDITIONAL_TEXT_ID)
) ENGINE = INNODB;

create table S19_CONTROL_CODE (
S19_CONTROL_CODE     SMALLINT                       not null,
BLOCK_TYPE           SMALLINT,
primary key (S19_CONTROL_CODE)
) ENGINE = INNODB;

create table SCHEMA_COLUMNS (
TABLE_CATALOG        VARCHAR(128)                   not null,
TABLE_SCHEMA         VARCHAR(128)                   not null,
TABLE_NAME           VARCHAR(128)                   not null,
COLUMN_NAME          VARCHAR(128)                   not null,
COLUMN_GUID          VARCHAR(128),
COLUMN_PROPID        INTEGER,
ORDINAL_POSITION     INTEGER,
COLUMN_HASDEFAULT    INTEGER,
COLUMN_DEFAULT       VARCHAR(128),
COLUMN_FLAGS         INTEGER,
IS_NULLABLE          INTEGER,
DATA_TYPE            INTEGER                        not null,
TYPE_GUID            VARCHAR(128),
CHARACTER_MAXIMUM_LENGTH INTEGER,
CHARACTER_OCTET_LENGTH INTEGER,
NUMERIC_PRECISION    INTEGER,
NUMERIC_SCALE        INTEGER,
DATETIME_PRECISION   INTEGER,
CHARACTER_SET_CATALOG VARCHAR(128),
CHARACTER_SET_SCHEMA VARCHAR(128),
CHARACTER_SET_NAME   VARCHAR(128),
COLLATION_CATALOG    VARCHAR(128),
COLLATION_SCHEMA     VARCHAR(128),
COLLATION_NAME       VARCHAR(128),
DOMAIN_CATALOG       VARCHAR(128),
DOMAIN_SCHEMA        VARCHAR(128),
DOMAIN_NAME          VARCHAR(128),
DESCRIPTION          VARCHAR(128),
primary key (TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME, COLUMN_NAME),
foreign key (TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME)
      references SCHEMA_TABLES (TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME)
) ENGINE = INNODB;

create table SCHEMA_COLUMNS_H (
TABLE_CATALOG        VARCHAR(128)                   not null,
TABLE_SCHEMA         VARCHAR(128)                   not null,
TABLE_NAME           VARCHAR(128)                   not null,
COLUMN_NAME          VARCHAR(128)                   not null,
COLUMN_GUID          VARCHAR(128),
COLUMN_PROPID        INTEGER,
ORDINAL_POSITION     INTEGER,
COLUMN_HASDEFAULT    INTEGER,
COLUMN_DEFAULT       VARCHAR(128),
COLUMN_FLAGS         INTEGER,
IS_NULLABLE          INTEGER,
DATA_TYPE            INTEGER                        not null,
TYPE_GUID            VARCHAR(128),
CHARACTER_MAXIMUM_LENGTH INTEGER,
CHARACTER_OCTET_LENGTH INTEGER,
NUMERIC_PRECISION    INTEGER,
NUMERIC_SCALE        INTEGER,
DATETIME_PRECISION   INTEGER,
CHARACTER_SET_CATALOG VARCHAR(128),
CHARACTER_SET_SCHEMA VARCHAR(128),
CHARACTER_SET_NAME   VARCHAR(128),
COLLATION_CATALOG    VARCHAR(128),
COLLATION_SCHEMA     VARCHAR(128),
COLLATION_NAME       VARCHAR(128),
DOMAIN_CATALOG       VARCHAR(128),
DOMAIN_SCHEMA        VARCHAR(128),
DOMAIN_NAME          VARCHAR(128),
DESCRIPTION          VARCHAR(128),
primary key (TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME, COLUMN_NAME),
foreign key (TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME)
      references SCHEMA_TABLES_H (TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME)
) ENGINE = INNODB;

create table ADDRESS (
ADDRESS_ID           INTEGER                        not null,
ADDRESS_FORMAT_ID    INTEGER                        not null,
TEXT_1               VARCHAR(40),
TEXT_2               VARCHAR(40),
TEXT_3               VARCHAR(40),
TEXT_4               VARCHAR(40),
TEXT_5               VARCHAR(40),
TEXT_6               VARCHAR(40),
TEXT_7               VARCHAR(40),
TEXT_8               VARCHAR(40),
TEXT_9               VARCHAR(40),
TEXT_10              VARCHAR(40),
TEXT_11              VARCHAR(40),
TEXT_12              VARCHAR(40),
TEXT_13              VARCHAR(40),
TEXT_14              VARCHAR(40),
TEXT_15              VARCHAR(40),
primary key (ADDRESS_ID),
foreign key (ADDRESS_FORMAT_ID)
      references ADDRESS_FORMAT (ADDRESS_FORMAT_ID)
) ENGINE = INNODB;

create table MEDIUM_CHANNEL (
MEDIUM_CHANNEL_ID    INTEGER                        not null,
MEDIUM_TYPE_NUMBER   SMALLINT                       not null,
CHANNEL_NUMBER       SMALLINT,
MAX_DEVICE_DUTY_CYCLE FLOAT,
primary key (MEDIUM_CHANNEL_ID),
foreign key (MEDIUM_TYPE_NUMBER)
      references MEDIUM_TYPE (MEDIUM_TYPE_NUMBER)
) ENGINE = INNODB;

create table MANUFACTURER (
MANUFACTURER_ID      INTEGER                        not null,
MANUFACTURER_NAME    VARCHAR(50),
ADDRESS_ID           INTEGER,
primary key (MANUFACTURER_ID),
foreign key (ADDRESS_ID)
      references ADDRESS (ADDRESS_ID)
) ENGINE = INNODB;

create table FUNCTIONAL_ENTITY (
FUNCTIONAL_ENTITY_ID INTEGER                        not null,
MANUFACTURER_ID      INTEGER,
FUNCTIONAL_ENTITY_NAME VARCHAR(50),
FUNCTIONAL_ENTITY_NUMB VARCHAR(20),
FUNCTIONAL_ENTITY_DESCRIPTION VARCHAR(80),
FUN_FUNCTIONAL_ENTITY_ID INTEGER,
HELP_FILE_NAME       VARCHAR(255),
CONTEXT_ID           INTEGER,
primary key (FUNCTIONAL_ENTITY_ID),
foreign key (FUN_FUNCTIONAL_ENTITY_ID)
      references FUNCTIONAL_ENTITY (FUNCTIONAL_ENTITY_ID),
foreign key (MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID)
) ENGINE = INNODB;

create table TEXT_ATTRIBUTE (
TEXT_ATTRIBUTE_ID    NUMERIC(6)                     not null,
LANGUAGE_ID          INTEGER                        not null,
COLUMN_ID            INTEGER                        not null,
ENTITY_ID            INTEGER                        not null,
TEXT_ATTRIBUTE_TEXT  VARCHAR(80),
primary key (TEXT_ATTRIBUTE_ID),
unique (LANGUAGE_ID, COLUMN_ID, ENTITY_ID),
foreign key (LANGUAGE_ID)
      references ETE_LANGUAGE (LANGUAGE_ID)
) ENGINE = INNODB;

create table DISTRIBUTION_LIST (
DISTRIBUTION_LIST_ID INTEGER                        not null,
DISTRIBUTION_LIST_NAME VARCHAR(50),
DISTRIBUTION_LIST_DESCRIPTION VARCHAR(80),
MANUFACTURER_ID      INTEGER                        not null,
DISTRIBUTION_LIST_NUMB VARCHAR(20),
primary key (DISTRIBUTION_LIST_ID),
foreign key (MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID)
) ENGINE = INNODB;

create table PERSON (
PERSON_ID            INTEGER                        not null,
ADDRESS_ID           INTEGER,
PERSON_NAME          VARCHAR(20),
PERSON_PHONE         VARCHAR(20),
PERSON_FAX           VARCHAR(20),
PERSON_DESCRIPTION   VARCHAR(80),
primary key (PERSON_ID),
foreign key (ADDRESS_ID)
      references ADDRESS (ADDRESS_ID)
) ENGINE = INNODB;

create table MIDDLEGROUP (
MIDDLEGROUP_ID       INTEGER                        not null,
MAINGROUP_ID         INTEGER,
MIDDLEGROUP_NAME     VARCHAR(50),
MIDDLEGROUP_ADDRESS  INTEGER,
MIDDLEGROUP_PASSTHROUGH SMALLINT,
MIDDLEGROUP_DESCRIPTION VARCHAR(80),
MIDDLEGROUP_LOCKED   INTEGER,
MIDDLEGROUP_UNIQUE_NAME VARCHAR(50),
LAST_SUBGROUP_ADDRESS SMALLINT,
primary key (MIDDLEGROUP_ID),
foreign key (MAINGROUP_ID)
      references MAINGROUP (MAINGROUP_ID)
) ENGINE = INNODB;

create table HW_PRODUCT (
PRODUCT_ID           INTEGER                        not null,
MANUFACTURER_ID      INTEGER                        not null,
SYMBOL_ID            INTEGER,
PRODUCT_NAME         VARCHAR(50),
PRODUCT_VERSION_NUMBER SMALLINT,
COMPONENT_TYPE       SMALLINT,
COMPONENT_ATTRIBUTES SMALLINT,
BUS_CURRENT          SMALLINT,
PRODUCT_SERIAL_NUMBER VARCHAR(30),
COMPONENT_TYPE_NUMBER SMALLINT,
PRODUCT_PICTURE      VARCHAR(255),
BCU_TYPE_NUMBER      SMALLINT,
PRODUCT_HANDLING     SMALLINT,
PRODUCT_DLL          VARCHAR(255),
ORIGINAL_MANUFACTURER_ID INTEGER,
primary key (PRODUCT_ID),
foreign key (BCU_TYPE_NUMBER)
      references BCU_TYPE (BCU_TYPE_NUMBER),
foreign key (COMPONENT_TYPE_NUMBER)
      references COMPONENT_TYPE (COMPONENT_TYPE_NUMBER),
foreign key (MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID),
foreign key (ORIGINAL_MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID),
foreign key (SYMBOL_ID)
      references SYMBOL (SYMBOL_ID)
) ENGINE = INNODB;

create table HELP_FILE (
HELP_FILE_ID         NUMERIC(6)                     not null,
LANGUAGE_ID          INTEGER                        not null,
ENTITY_ID            INTEGER                        not null,
HELP_FILE_NAME       VARCHAR(255),
primary key (HELP_FILE_ID),
foreign key (LANGUAGE_ID)
      references ETE_LANGUAGE (LANGUAGE_ID)
) ENGINE = INNODB;

create table EIB_DATA_SUBTYPE (
EIB_DATA_TYPE_CODE   SMALLINT                       not null,
EIB_DATA_SUBTYPE_CODE SMALLINT                       not null,
EIB_DATA_SUBTYPE_NAME VARCHAR(50),
EIB_DATA_SUBTYPE_TEXT VARCHAR(50),
ALLOWED_FOR_COMM_OBJECTS SMALLINT,
primary key (EIB_DATA_TYPE_CODE, EIB_DATA_SUBTYPE_CODE),
foreign key (EIB_DATA_TYPE_CODE)
      references EIB_DATA_TYPE (EIB_DATA_TYPE_CODE)
) ENGINE = INNODB;

create table PROJECT (
PROJECT_ID           INTEGER                        not null,
PROJECT_RESPONSIBLE_ID INTEGER,
PROJECT_ADDRESS_ID   INTEGER,
CUSTOMER_ADDRESS_ID  INTEGER,
PROJECT_NAME         VARCHAR(50),
PROJECT_NUMB         VARCHAR(20),
CONTRACT_NUMBER      VARCHAR(20),
BEGIN_DATE           DATE,
END_DATE             DATE,
CONVERSION_DATE      DATE,
IMPORT_DATE          DATE,
PROJECT_LOCKED       INTEGER,
PROJECT_BCU_PASSWORD INTEGER,
LAST_AREA_ADDRESS    SMALLINT,
LAST_MAINGROUP_ADDRESS SMALLINT,
PROJECT_PASSWORD     VARCHAR(20),
HOUSE_ADDRESS        SMALLINT,
CHANNEL              SMALLINT,
SYSTEM_ID            SMALLINT,
SYSTEM_ID_SCAN       SMALLINT,
REPEATER_MODE        SMALLINT,
primary key (PROJECT_ID),
foreign key (PROJECT_ADDRESS_ID)
      references ADDRESS (ADDRESS_ID),
foreign key (CUSTOMER_ADDRESS_ID)
      references ADDRESS (ADDRESS_ID),
foreign key (PROJECT_RESPONSIBLE_ID)
      references PERSON (PERSON_ID)
) ENGINE = INNODB;

create table CHANNEL_LIST (
CHANNEL_LIST_ID      INTEGER                        not null,
MEDIUM_TYPE_NUMBER   SMALLINT                       not null,
CHANNEL_LIST_NAME    VARCHAR(255),
primary key (CHANNEL_LIST_ID),
foreign key (MEDIUM_TYPE_NUMBER)
      references MEDIUM_TYPE (MEDIUM_TYPE_NUMBER)
) ENGINE = INNODB;

create table BUILDING (
BUILDING_ID          INTEGER                        not null,
PROJECT_ID           INTEGER,
BUI_BUILDING_ID      INTEGER,
BUILDING_NAME        VARCHAR(50),
BUILDING_NUMB        VARCHAR(20),
BUILDING_DESCRIPTION VARCHAR(80),
BUILDING_LOCKED      INTEGER,
BUILDING_UNIQUE_NAME VARCHAR(50),
primary key (BUILDING_ID),
foreign key (BUI_BUILDING_ID)
      references BUILDING (BUILDING_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table SPECIFICATION_TEXT (
SPEC_TEXT_ID         INTEGER                        not null,
MANUFACTURER_ID      INTEGER,
SPEC_TEXT_NAME       VARCHAR(50),
SPEC_TEXT_NUMB       VARCHAR(20),
SPEC_TEXT_FILENAME   VARCHAR(64),
primary key (SPEC_TEXT_ID),
foreign key (MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID)
) ENGINE = INNODB;

create table SUBGROUP (
MIDDLEGROUP_ID       INTEGER,
SUBGROUP_TYPE        SMALLINT,
SUBGROUP_ID          INTEGER                        not null,
SUBGROUP_NAME        VARCHAR(50),
SUBGROUP_ADDRESS     INTEGER,
SUBGROUP_DESCRIPTION VARCHAR(80),
SUBGROUP_PASSTHROUGH SMALLINT,
SUBGROUP_LOCKED      INTEGER,
SUBGROUP_UNIQUE_NAME VARCHAR(50),
SUBGROUP_CENTRAL     SMALLINT,
primary key (SUBGROUP_ID),
foreign key (MIDDLEGROUP_ID)
      references MIDDLEGROUP (MIDDLEGROUP_ID)
) ENGINE = INNODB;

create table PRODUCT_CATALOG (
PRODUCT_CATALOG_ID   NUMERIC(6)                     not null,
MANUFACTURER_ID      INTEGER                        not null,
PRODUCT_CATALOG_NUMB VARCHAR(20),
PRODUCT_CATALOG_DESCRIPTION VARCHAR(80),
primary key (PRODUCT_CATALOG_ID),
foreign key (MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID)
) ENGINE = INNODB;

create table PROJECT_CHECK (
PROJECT_ID           INTEGER                        not null,
ERROR_CATEGORY       VARCHAR(80),
ERROR_ADDRESS        VARCHAR(80),
ERROR_NAME           VARCHAR(80),
ERROR_DESCRIPTION    VARCHAR(80),
ERROR_ID             INTEGER                        not null,
ERROR_CODE           INTEGER,
ERROR_ENTITY_ID      INTEGER,
ERROR_TABLE_ID       INTEGER,
primary key (ERROR_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table PROJECT_CONFIG (
PROJECT_ID           INTEGER                        not null,
VAT_RATE             NUMERIC(3,1),
CURRENCY             VARCHAR(3),
primary key (PROJECT_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table MANUFACTURER_COMPATIBILITY (
MANUFACTURER_COMPATIBILITY_ID NUMERIC(6)                     not null,
MANUFACTURER_ID      INTEGER                        not null,
MAN_MANUFACTURER_ID  INTEGER                        not null,
primary key (MANUFACTURER_COMPATIBILITY_ID),
foreign key (MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID),
foreign key (MAN_MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID)
) ENGINE = INNODB;

create table MASK (
MASK_ID              INTEGER                        not null,
MASK_VERSION         INTEGER,
USER_RAM_START       INTEGER,
USER_RAM_END         INTEGER,
USER_EEPROM_START    INTEGER,
USER_EEPROM_END      INTEGER,
RUN_ERROR_ADDRESS    INTEGER,
ADDRESS_TAB_ADDRESS  INTEGER,
ASSOCTABPTR_ADDRESS  INTEGER,
COMMSTABPTR_ADDRESS  INTEGER,
MANUFACTURER_DATA_ADDRESS INTEGER,
MANUFACTURER_DATA_SIZE INTEGER,
MANUFACTURER_ID_ADDRESS INTEGER,
ROUTECNT_ADDRESS     INTEGER,
MANUFACTURER_ID_PROTECTED SMALLINT,
MASK_VERSION_NAME    VARCHAR(50),
MASK_EEPROM_DATA     LONG VARBINARY,
MASK_DATA_LENGTH     INTEGER,
ADDRESS_TAB_LCS      INTEGER,
ASSOC_TAB_LCS        INTEGER,
APPLICATION_PROGRAM_LCS INTEGER,
PEI_PROGRAM_LCS      INTEGER,
LOAD_CONTROL_ADDRESS INTEGER,
RUN_CONTROL_ADDRESS  INTEGER,
EXTERNAL_MEMORY_START INTEGER,
EXTERNAL_MEMORY_END  INTEGER,
APPLICATION_PROGRAM_RCS INTEGER,
PEI_PROGRAM_RCS      INTEGER,
PORT_A_DDR           INTEGER,
PORT_ADDRESS_PROTECTED SMALLINT,
MEDIUM_TYPE_NUMBER   SMALLINT,
MEDIUM_TYPE_NUMBER2  SMALLINT,
BCU_TYPE_NUMBER      SMALLINT,
primary key (MASK_ID),
foreign key (BCU_TYPE_NUMBER)
      references BCU_TYPE (BCU_TYPE_NUMBER),
foreign key (MEDIUM_TYPE_NUMBER)
      references MEDIUM_TYPE (MEDIUM_TYPE_NUMBER),
foreign key (MEDIUM_TYPE_NUMBER2)
      references MEDIUM_TYPE (MEDIUM_TYPE_NUMBER)
) ENGINE = INNODB;

create table SYSTEM_ID_LOOKUP (
SYSTEM_ID_LOOKUP_ID  INTEGER                        not null,
PROJECT_ID           INTEGER,
SYSTEM_ID            SMALLINT,
MEDIUM_TYPE_NUMBER   SMALLINT,
primary key (SYSTEM_ID_LOOKUP_ID),
foreign key (MEDIUM_TYPE_NUMBER)
      references MEDIUM_TYPE (MEDIUM_TYPE_NUMBER),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table SYSID_SCAN (
SYSID_SCAN_ID        INTEGER                        not null,
PROJECT_ID           INTEGER,
SYSTEM_ID            SMALLINT,
MEDIUM_TYPE_NUMBER   SMALLINT,
CHANNEL_LIST_ID      INTEGER,
primary key (SYSID_SCAN_ID),
foreign key (CHANNEL_LIST_ID)
      references CHANNEL_LIST (CHANNEL_LIST_ID),
foreign key (MEDIUM_TYPE_NUMBER)
      references MEDIUM_TYPE (MEDIUM_TYPE_NUMBER),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table ADDITIONAL_FILE (
ADDITIONAL_FILE_ID   INTEGER                        not null,
PROJECT_ID           INTEGER,
ADDITIONAL_FILE_NAME VARCHAR(12),
primary key (ADDITIONAL_FILE_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table FUNCTION (
FUNCTION_ID          INTEGER                        not null,
PROJECT_ID           INTEGER,
FUN_FUNCTION_ID      INTEGER,
FUNCTION_NAME        VARCHAR(50),
FUNCTION_NUMB        VARCHAR(20),
FUNCTION_DESCRIPTION VARCHAR(80),
FUNCTION_LOCKED      INTEGER,
FUNCTION_UNIQUE_NAME VARCHAR(50),
primary key (FUNCTION_ID),
foreign key (FUN_FUNCTION_ID)
      references FUNCTION (FUNCTION_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table MASK_COMPATIBILITY (
MASK_COMPATIBILITY_ID NUMERIC(6)                     not null,
OLD_MASK_ID          INTEGER                        not null,
NEW_MASK_ID          INTEGER                        not null,
STATUS               SMALLINT,
primary key (MASK_COMPATIBILITY_ID),
foreign key (OLD_MASK_ID)
      references MASK (MASK_ID),
foreign key (NEW_MASK_ID)
      references MASK (MASK_ID)
) ENGINE = INNODB;

create table AREA (
AREA_ID              INTEGER                        not null,
PROJECT_ID           INTEGER,
AREA_NAME            VARCHAR(50),
AREA_ADDRESS         INTEGER,
AREA_DESCRIPTION     VARCHAR(80),
AREA_FILTER_MODIFIED SMALLINT,
AREA_LOCKED          INTEGER,
AREA_UNIQUE_NAME     VARCHAR(50),
LAST_LINE_ADDRESS    SMALLINT,
SYSTEM_ID            SMALLINT,
SYSTEM_ID_SCAN       SMALLINT,
REPEATER_MODE        SMALLINT,
primary key (AREA_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table PRODUCT_ATTR (
PRODUCT_ATTR_ID      INTEGER                        not null,
HW_PRODUCT_ID        INTEGER                        not null,
ATTR_ID              SMALLINT                       not null,
ATTR_TYPE            SMALLINT                       not null,
ATTR_VALUE           VARCHAR(255),
primary key (PRODUCT_ATTR_ID),
foreign key (HW_PRODUCT_ID)
      references HW_PRODUCT (PRODUCT_ID)
) ENGINE = INNODB;

create table MASK_SYSTEM_POINTER (
MASK_SYSTEM_POINTER_ID NUMERIC(6)                     not null,
MASK_ID              INTEGER,
SYSTEM_POINTER_NAME  VARCHAR(40),
SYSTEM_POINTER_ADDRESS INTEGER,
primary key (MASK_SYSTEM_POINTER_ID),
foreign key (MASK_ID)
      references MASK (MASK_ID)
) ENGINE = INNODB;

create table CHANNEL_LIST_TO_CHANNEL (
CHANNEL_LIST_TO_CHANNEL_ID INTEGER                        not null,
CHANNEL_LIST_ID      INTEGER                        not null,
MEDIUM_CHANNEL_ID    INTEGER                        not null,
primary key (CHANNEL_LIST_TO_CHANNEL_ID),
foreign key (CHANNEL_LIST_ID)
      references CHANNEL_LIST (CHANNEL_LIST_ID),
foreign key (MEDIUM_CHANNEL_ID)
      references MEDIUM_CHANNEL (MEDIUM_CHANNEL_ID)
) ENGINE = INNODB;

create table MASK_ENTRY (
MASK_ENTRY_ID        INTEGER                        not null,
MASK_ID              INTEGER,
MASK_ENTRY_NAME      VARCHAR(50),
MASK_ENTRY_ADDRESS   SMALLINT,
primary key (MASK_ENTRY_ID),
foreign key (MASK_ID)
      references MASK (MASK_ID)
) ENGINE = INNODB;

create table ROOM (
ROOM_ID              INTEGER                        not null,
BUILDING_ID          INTEGER,
ROOM_NAME            VARCHAR(50),
ROOM_NUMB            VARCHAR(20),
ROOM_OUTLINE         VARCHAR(1),
ROOM_DESCRIPTION     VARCHAR(80),
PROJECT_ID           INTEGER,
ROOM_LOCKED          INTEGER,
ROOM_UNIQUE_NAME     VARCHAR(50),
primary key (ROOM_ID),
foreign key (BUILDING_ID)
      references BUILDING (BUILDING_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table APPLICATION_PROGRAM (
PROGRAM_ID           INTEGER                        not null,
SYMBOL_ID            INTEGER,
MASK_ID              INTEGER,
PROGRAM_NAME         VARCHAR(50),
PROGRAM_VERSION      VARCHAR(5),
PROGRAM_VERSION_NUMBER SMALLINT,
LINKABLE             SMALLINT,
DEVICE_TYPE          INTEGER,
PEI_TYPE             SMALLINT,
ADDRESS_TAB_SIZE     INTEGER,
ASSOCTAB_ADDRESS     INTEGER,
ASSOCTAB_SIZE        INTEGER,
COMMSTAB_ADDRESS     INTEGER,
COMMSTAB_SIZE        INTEGER,
PROGRAM_SERIAL_NUMBER VARCHAR(20),
MANUFACTURER_ID      INTEGER,
EEPROM_DATA          LONG VARBINARY,
DATA_LENGTH          INTEGER,
S19_FILE             VARCHAR(255),
MAP_FILE             VARCHAR(255),
ASSEMBLER_ID         INTEGER,
HELP_FILE_NAME       VARCHAR(255),
CONTEXT_ID           INTEGER,
DYNAMIC_MANAGEMENT   SMALLINT,
PROGRAM_TYPE         SMALLINT,
RAM_SIZE             INTEGER,
ORIGINAL_MANUFACTURER_ID INTEGER,
API_VERSION          INTEGER,
PROGRAM_STYLE        SMALLINT,
IS_POLLING_MASTER    SMALLINT,
NUMBER_OF_POLLING_GROUPS SMALLINT,
primary key (PROGRAM_ID),
foreign key (ASSEMBLER_ID)
      references ASSEMBLER (ASSEMBLER_ID),
foreign key (MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID),
foreign key (ORIGINAL_MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID),
foreign key (MASK_ID)
      references MASK (MASK_ID),
foreign key (SYMBOL_ID)
      references SYMBOL (SYMBOL_ID)
) ENGINE = INNODB;

create table S19_BLOCK (
BLOCK_ID             INTEGER                        not null,
BLOCK_NUMBER         SMALLINT,
PROGRAM_ID           INTEGER                        not null,
BLOCK_NAME           VARCHAR(50),
BLOCK_TYPE           SMALLINT,
CONTROL_CODE         SMALLINT,
SEGMENT_TYPE         SMALLINT,
SEGMENT_ID           SMALLINT,
SEGMENT_ADDRESS      INTEGER,
SEGMENT_LENGTH       INTEGER,
ACCESS_ATTRIBUTES    SMALLINT,
MEMORY_TYPE          SMALLINT,
MEMORY_ATTRIBUTES    SMALLINT,
BLOCK_DATA           LONG VARBINARY,
BLOCK_MASK           LONG VARBINARY,
primary key (BLOCK_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID)
) ENGINE = INNODB;

create table DEVICE_INFO (
DEVICE_INFO_ID       INTEGER                        not null,
MASK_ID              INTEGER                        not null,
DEV_DEVICE_INFO_ID   INTEGER,
DEVICE_INFO_NAME     VARCHAR(50),
FUNCTION_NUMBER      SMALLINT,
DEVICE_INFO_TYPE     SMALLINT,
BIT_POSITION         SMALLINT,
DEVICE_INFO_VISIBLE  SMALLINT,
primary key (DEVICE_INFO_ID),
foreign key (DEV_DEVICE_INFO_ID)
      references DEVICE_INFO (DEVICE_INFO_ID),
foreign key (MASK_ID)
      references MASK (MASK_ID)
) ENGINE = INNODB;

create table PROGRAM_TO_MASK_FEATURE (
PROGRAM_TO_MASK_FEATURE_ID INTEGER                        not null,
PROGRAM_ID           INTEGER                        not null,
MASK_FEATURE_ID      INTEGER                        not null,
MASK_FEATURE_VALUE   INTEGER,
primary key (PROGRAM_TO_MASK_FEATURE_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (MASK_FEATURE_ID)
      references MASK_FEATURE (MASK_FEATURE_ID)
) ENGINE = INNODB;

create table TENDER (
TENDER_ID            INTEGER                        not null,
PROJECT_ID           INTEGER,
TENDER_NAME          VARCHAR(50),
TENDER_NUMB          VARCHAR(20),
TENDER_DATE          DATE,
TENDER_VALIDITY      DATE,
TENDER_DELIVERY_DATE DATE,
ADDITIONAL_TEXT_ID   INTEGER,
ADD_ADDITIONAL_TEXT_ID INTEGER,
ADD2_ADDITIONAL_TEXT_ID INTEGER,
ADD3_ADDITIONAL_TEXT_ID INTEGER,
primary key (TENDER_ID),
foreign key (ADDITIONAL_TEXT_ID)
      references ADDITIONAL_TEXT (ADDITIONAL_TEXT_ID),
foreign key (ADD_ADDITIONAL_TEXT_ID)
      references ADDITIONAL_TEXT (ADDITIONAL_TEXT_ID),
foreign key (ADD2_ADDITIONAL_TEXT_ID)
      references ADDITIONAL_TEXT (ADDITIONAL_TEXT_ID),
foreign key (ADD3_ADDITIONAL_TEXT_ID)
      references ADDITIONAL_TEXT (ADDITIONAL_TEXT_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table DEVICE_INFO_VALUE (
DEVICE_INFO_VALUE_ID INTEGER                        not null,
DEVICE_INFO_ID       INTEGER                        not null,
BITMAP_ID            INTEGER,
MINIMUM_VALUE        INTEGER,
MAXIMUM_VALUE        INTEGER,
DISPLAYED_VALUE      VARCHAR(50),
primary key (DEVICE_INFO_VALUE_ID),
foreign key (DEVICE_INFO_ID)
      references DEVICE_INFO (DEVICE_INFO_ID)
) ENGINE = INNODB;

create table MASK_TO_MASK_FEATURE (
MASK_TO_MASK_FEATURE_ID INTEGER                        not null,
MASK_ID              INTEGER                        not null,
MASK_FEATURE_ID      INTEGER                        not null,
MASK_FEATURE_VALUE   INTEGER,
primary key (MASK_TO_MASK_FEATURE_ID),
foreign key (MASK_ID)
      references MASK (MASK_ID),
foreign key (MASK_FEATURE_ID)
      references MASK_FEATURE (MASK_FEATURE_ID)
) ENGINE = INNODB;

create table PRODUCT_TO_PROGRAM (
PROD2PROG_ID         INTEGER                        not null,
PRODUCT_ID           INTEGER,
PROGRAM_ID           INTEGER,
PROD2PROG_STATUS     VARCHAR(40),
PEI_PROGRAM_ID       INTEGER,
PROD2PROG_STATUS_CODE SMALLINT,
REGISTRATION_NUMBER  INTEGER,
REGISTRATION_YEAR    SMALLINT,
ORIGINAL_REGISTRATION_NUMBER INTEGER,
ORIGINAL_REGISTRATION_YEAR SMALLINT,
EXPIRATION_DATE      DATE,
REGISTRATION_TS      TIMESTAMP,
REGISTRATION_DATE    DATE,
REGISTRATION_COMMENT VARCHAR(50),
REGISTRATION_MEDIUM_TYPE VARCHAR(20),
DCY_F_INDIVIDUAL     FLOAT,
DCY_G_INDIVIDUAL     FLOAT,
DCY_F_GROUP          FLOAT,
DCY_G_GROUP          FLOAT,
primary key (PROD2PROG_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (PEI_PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (PRODUCT_ID)
      references HW_PRODUCT (PRODUCT_ID)
) ENGINE = INNODB;

create table ADDRESS_FIXUP (
FIXUP_ID             INTEGER                        not null,
PROGRAM_ID           INTEGER,
FIXUP_TYPE           SMALLINT,
FIXUP_NAME           VARCHAR(50),
FIXUP_ADDRESS        SMALLINT,
primary key (FIXUP_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID)
) ENGINE = INNODB;

create table EFFORT_TO_TENDER (
EFFORT_TO_TENDER_ID  INTEGER                        not null,
EFFORT_ID            INTEGER,
TENDER_ID            INTEGER,
HOURS                SMALLINT,
MINUTES              SMALLINT,
SECONDS              SMALLINT,
WAGE                 NUMERIC(12,3),
PRICE                NUMERIC(12,3),
primary key (EFFORT_TO_TENDER_ID),
foreign key (EFFORT_ID)
      references ADDITIONAL_EFFORT (EFFORT_ID),
foreign key (TENDER_ID)
      references TENDER (TENDER_ID)
) ENGINE = INNODB;

create table PARAMETER_TYPE (
PARAMETER_TYPE_ID    INTEGER                        not null,
ATOMIC_TYPE_NUMBER   SMALLINT,
PROGRAM_ID           INTEGER,
PARAMETER_TYPE_NAME  VARCHAR(50),
PARAMETER_MINIMUM_VALUE INTEGER,
PARAMETER_MAXIMUM_VALUE INTEGER,
PARAMETER_TYPE_DESCRIPTION VARCHAR(80),
PARAMETER_TYPE_LOW_ACCESS SMALLINT,
PARAMETER_TYPE_HIGH_ACCESS SMALLINT,
PARAMETER_TYPE_SIZE  SMALLINT,
PARAMETER_MINIMUM_DOUBLE_VALUE DOUBLE PRECISION,
PARAMETER_MAXIMUM_DOUBLE_VALUE DOUBLE PRECISION,
PARAMETER_UI_HINT    VARCHAR(40),
primary key (PARAMETER_TYPE_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (ATOMIC_TYPE_NUMBER)
      references PARAMETER_ATOMIC_TYPE (ATOMIC_TYPE_NUMBER)
) ENGINE = INNODB;

create table PRODUCT_TO_PROGRAM_TO_MT (
PROD2PROG2MT_ID      INTEGER                        not null,
PROD2PROG_ID         INTEGER                        not null,
MEDIUM_TYPE_NUMBER   SMALLINT                       not null,
CHANNEL_LIST_ID      INTEGER,
primary key (PROD2PROG2MT_ID),
foreign key (CHANNEL_LIST_ID)
      references CHANNEL_LIST (CHANNEL_LIST_ID),
foreign key (MEDIUM_TYPE_NUMBER)
      references MEDIUM_TYPE (MEDIUM_TYPE_NUMBER),
foreign key (PROD2PROG_ID)
      references PRODUCT_TO_PROGRAM (PROD2PROG_ID)
) ENGINE = INNODB;

create table PROGRAM_DESCRIPTION (
PROGRAM_DESCRIPTION_ID INTEGER                        not null,
PROGRAM_ID           INTEGER,
TEXT                 VARCHAR(1000),
DISPLAY_ORDER        SMALLINT,
LANGUAGE_ID          INTEGER,
primary key (PROGRAM_DESCRIPTION_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (LANGUAGE_ID)
      references ETE_LANGUAGE (LANGUAGE_ID)
) ENGINE = INNODB;

create table PROGRAM_PLUGIN (
PROGRAM_PLUGIN_ID    INTEGER                        not null,
PROGRAM_ID           INTEGER                        not null,
PLUGIN_TYPE          SMALLINT                       not null,
PLUGIN_CLSID         VARCHAR(40)                    not null,
PLUGIN_CODEBASE      VARCHAR(255),
primary key (PROGRAM_PLUGIN_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID)
) ENGINE = INNODB;

create table PROJECT_HISTORY (
PROJECT_HISTORY_ID   INTEGER                        not null,
PROJECT_ID           INTEGER                        not null,
PROJECT_HISTORY_DATE TIMESTAMP,
PROJECT_HISTORY_TEXT VARCHAR(80),
primary key (PROJECT_HISTORY_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID)
) ENGINE = INNODB;

create table CALCULATION (
TENDER_ID            INTEGER                        not null,
WAGE_ENGINEERING     NUMERIC(12,3),
WAGE_MOUNTING        NUMERIC(12,3),
VAT_RATE             NUMERIC(3,1),
SUM_DEVICE           NUMERIC(12,3),
PERCENTAGE           NUMERIC(5,2),
SUM_MOUNTING         NUMERIC(12,3),
TOTAL                NUMERIC(12,3),
SUM_ADDITIONAL_EFFORTS NUMERIC(12,3),
primary key (TENDER_ID),
foreign key (TENDER_ID)
      references TENDER (TENDER_ID)
) ENGINE = INNODB;

create table PARAMETER (
PROGRAM_ID           INTEGER                        not null,
PARAMETER_TYPE_ID    INTEGER,
PARAMETER_NUMBER     SMALLINT,
PARAMETER_NAME       VARCHAR(50),
PARAMETER_LOW_ACCESS SMALLINT,
PARAMETER_HIGH_ACCESS SMALLINT,
PARENT_PARM_VALUE    INTEGER,
PARAMETER_SIZE       SMALLINT,
PARAMETER_FUNCTION   VARCHAR(30),
PARAMETER_DISPLAY_ORDER SMALLINT,
PARAMETER_ADDRESS    INTEGER,
PARAMETER_BITOFFSET  SMALLINT,
PARAMETER_DESCRIPTION VARCHAR(80),
PARAMETER_ID         INTEGER                        not null,
PAR_PARAMETER_ID     INTEGER,
PARAMETER_LABEL      VARCHAR(30),
PARAMETER_DEFAULT_LONG INTEGER,
PARAMETER_DEFAULT_STRING VARCHAR(255),
CONTEXT_ID           INTEGER,
PARAMETER_DEFAULT_DOUBLE DOUBLE PRECISION,
PATCH_ALWAYS         SMALLINT,
ADDRESS_SPACE        SMALLINT,
EIB_OBJECT_REF       SMALLINT,
EIB_PROPERTY_ID      INTEGER,
primary key (PARAMETER_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (PAR_PARAMETER_ID)
      references PARAMETER (PARAMETER_ID),
foreign key (PARAMETER_TYPE_ID)
      references PARAMETER_TYPE (PARAMETER_TYPE_ID)
) ENGINE = INNODB;

create table CATALOG_ENTRY (
CATALOG_ENTRY_ID     INTEGER                        not null,
PRODUCT_ID           INTEGER                        not null,
MANUFACTURER_ID      INTEGER                        not null,
SYMBOL_ID            INTEGER,
ORDER_NUMBER         VARCHAR(20),
ENTRY_NAME           VARCHAR(50),
ENTRY_COLOUR         VARCHAR(20),
ENTRY_WIDTH_IN_MODULES NUMERIC(7,2),
ENTRY_WIDTH_IN_MILLIMETERS NUMERIC(7,2),
PRICE                NUMERIC(12,3),
CURRENCY             VARCHAR(3),
QUANTITY_UNIT        VARCHAR(10),
MATERIAL_PRICE       NUMERIC(12,3),
MOUNTING_HOURS       SMALLINT,
MOUNTING_MINUTES     SMALLINT,
MOUNTING_SECONDS     SMALLINT,
DIN_FLAG             SMALLINT,
SERIES               VARCHAR(20),
CATALOG_NAME         VARCHAR(50),
PAGE_NUMBER          SMALLINT,
ENTRY_PICTURE        VARCHAR(255),
DESIGNATION_TYPE     VARCHAR(1),
DESIGNATION_FUNCTION VARCHAR(2),
HELP_FILE_NAME       VARCHAR(255),
CONTEXT_ID           INTEGER,
RAM_SIZE             INTEGER,
REGISTRATION_NUMBER  INTEGER,
REGISTRATION_YEAR    SMALLINT,
ENTRY_STATUS_CODE    SMALLINT,
REGISTRATION_TS      TIMESTAMP,
REGISTRATION_DATE    DATE,
REGISTRATION_COMMENT VARCHAR(50),
primary key (CATALOG_ENTRY_ID),
foreign key (PRODUCT_ID)
      references HW_PRODUCT (PRODUCT_ID),
foreign key (MANUFACTURER_ID)
      references MANUFACTURER (MANUFACTURER_ID),
foreign key (SYMBOL_ID)
      references SYMBOL (SYMBOL_ID)
) ENGINE = INNODB;

create table SYMBOL_CONDITION (
SYMBOL_CONDITION_ID  INTEGER                        not null,
SYMBOL_ID            INTEGER,
PARENT_PARM_VALUE    INTEGER,
PARAMETER_ID         INTEGER                        not null,
primary key (SYMBOL_CONDITION_ID),
foreign key (PARAMETER_ID)
      references PARAMETER (PARAMETER_ID),
foreign key (SYMBOL_ID)
      references SYMBOL (SYMBOL_ID)
) ENGINE = INNODB;

create table S19_BLOCK_PARAGRAPH (
S19_BLOCK_PARAGRAPH_ID INTEGER                        not null,
BLOCK_ID             INTEGER                        not null,
PT_COLUMN_ID         SMALLINT,
DATA_LONG            INTEGER,
DATA_BINARY          BINARY(16),
primary key (S19_BLOCK_PARAGRAPH_ID),
foreign key (BLOCK_ID)
      references S19_BLOCK (BLOCK_ID)
) ENGINE = INNODB;

create table VIRTUAL_DEVICE (
VIRTUAL_DEVICE_ID    INTEGER                        not null,
SYMBOL_ID            INTEGER,
CATALOG_ENTRY_ID     INTEGER                        not null,
PROGRAM_ID           INTEGER,
VIRTUAL_DEVICE_NAME  VARCHAR(50),
VIRTUAL_DEVICE_DESCRIPTION VARCHAR(80),
FUNCTIONAL_ENTITY_ID INTEGER,
PRODUCT_TYPE_ID      INTEGER,
HELP_FILE_NAME       VARCHAR(255),
CONTEXT_ID           INTEGER,
PEI_PROGRAM_ID       INTEGER,
VIRTUAL_DEVICE_NUMBER SMALLINT,
MEDIUM_TYPES         VARCHAR(255),
primary key (VIRTUAL_DEVICE_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (PEI_PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (CATALOG_ENTRY_ID)
      references CATALOG_ENTRY (CATALOG_ENTRY_ID),
foreign key (FUNCTIONAL_ENTITY_ID)
      references FUNCTIONAL_ENTITY (FUNCTIONAL_ENTITY_ID),
foreign key (PRODUCT_TYPE_ID)
      references FUNCTIONAL_ENTITY (FUNCTIONAL_ENTITY_ID),
foreign key (SYMBOL_ID)
      references SYMBOL (SYMBOL_ID)
) ENGINE = INNODB;

create table PARAMETER_LIST_OF_VALUES (
PARAMETER_TYPE_ID    INTEGER                        not null,
REAL_VALUE           INTEGER,
DISPLAYED_VALUE      VARCHAR(50),
DISPLAY_ORDER        SMALLINT,
PARAMETER_VALUE_ID   INTEGER                        not null,
BINARY_VALUE         LONG VARBINARY,
BINARY_VALUE_LENGTH  INTEGER,
DOUBLE_VALUE         DOUBLE PRECISION,
primary key (PARAMETER_VALUE_ID),
foreign key (PARAMETER_TYPE_ID)
      references PARAMETER_TYPE (PARAMETER_TYPE_ID)
) ENGINE = INNODB;

create table COMMUNICATION_OBJECT (
PROGRAM_ID           INTEGER,
OBJECT_NAME          VARCHAR(50),
OBJECT_FUNCTION      VARCHAR(30),
OBJECT_READENABLED   SMALLINT,
OBJECT_WRITEENABLED  SMALLINT,
OBJECT_COMMENABLED   SMALLINT,
OBJECT_TRANSENABLED  SMALLINT,
OBJECT_DISPLAY_ORDER INTEGER,
PARENT_PARAMETER_VALUE INTEGER,
OBJECT_ID            INTEGER                        not null,
PARAMETER_ID         INTEGER,
OBJECT_NUMBER        SMALLINT,
OBJECT_DESCRIPTION   VARCHAR(80),
OBJECT_TYPE          SMALLINT,
OBJECT_PRIORITY      SMALLINT,
OBJECT_UPDATEENABLED SMALLINT,
OBJECT_UNIQUE_NUMBER SMALLINT,
EIB_DATA_TYPE_CODE   SMALLINT,
EIB_DATA_SUBTYPE_CODE SMALLINT,
primary key (OBJECT_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (EIB_DATA_TYPE_CODE, EIB_DATA_SUBTYPE_CODE)
      references EIB_DATA_SUBTYPE (EIB_DATA_TYPE_CODE, EIB_DATA_SUBTYPE_CODE),
foreign key (EIB_DATA_TYPE_CODE)
      references EIB_DATA_TYPE (EIB_DATA_TYPE_CODE),
foreign key (OBJECT_PRIORITY)
      references OBJECT_PRIORITY (OBJECT_PRIORITY_CODE),
foreign key (OBJECT_TYPE)
      references OBJECT_TYPE (OBJECT_TYPE_CODE),
foreign key (PARAMETER_ID)
      references PARAMETER (PARAMETER_ID)
) ENGINE = INNODB;

create table ACCESSORY (
ACCESSORY_ID         INTEGER                        not null,
CATALOG_ENTRY_ID     INTEGER                        not null,
CAT_CATALOG_ENTRY_ID INTEGER                        not null,
ACC_QUANTITY         INTEGER,
primary key (ACCESSORY_ID),
foreign key (CATALOG_ENTRY_ID)
      references CATALOG_ENTRY (CATALOG_ENTRY_ID),
foreign key (CAT_CATALOG_ENTRY_ID)
      references CATALOG_ENTRY (CATALOG_ENTRY_ID)
) ENGINE = INNODB;

create table DEVICE_OBJECT (
DEVICE_ID            INTEGER,
OBJECT_PRIO          SMALLINT,
OBJECT_READ          SMALLINT,
OBJECT_WRITE         SMALLINT,
OBJECT_COMM          SMALLINT,
OBJECT_TRANS         SMALLINT,
OBJECT_SUBGROUPS_TEXT VARCHAR(255),
DEVICE_OBJECT_ID     INTEGER                        not null,
OBJECT_ID            INTEGER,
DEVICE_OBJECT_NUMBER SMALLINT,
OBJECT_SUBGROUPS_TEXT2 VARCHAR(255),
DEVICE_OBJECT_VISIBLE SMALLINT,
DEVICE_OBJECT_UNIQUE_NAME VARCHAR(50),
OBJECT_UPDATE        SMALLINT,
DEVICE_OBJECT_UNIQUE_NUMBER SMALLINT,
DEVICE_OBJECT_TYPE   SMALLINT,
EIB_DATA_TYPE_CODE   SMALLINT,
EIB_DATA_SUBTYPE_CODE SMALLINT,
primary key (DEVICE_OBJECT_ID),
foreign key (OBJECT_ID)
      references COMMUNICATION_OBJECT (OBJECT_ID),
foreign key (EIB_DATA_TYPE_CODE, EIB_DATA_SUBTYPE_CODE)
      references EIB_DATA_SUBTYPE (EIB_DATA_TYPE_CODE, EIB_DATA_SUBTYPE_CODE),
foreign key (EIB_DATA_TYPE_CODE)
      references EIB_DATA_TYPE (EIB_DATA_TYPE_CODE),
foreign key (DEVICE_OBJECT_TYPE)
      references OBJECT_TYPE (OBJECT_TYPE_CODE)
) ENGINE = INNODB;

create table EIB_OBJECT (
EIB_OBJECT_ID        INTEGER                        not null,
MASK_ID              INTEGER,
PROGRAM_ID           INTEGER,
EIB_OBJECT_NUMBER    SMALLINT,
EIB_OBJECT_NAME      VARCHAR(50),
EIB_OBJECT_DESCRIPTION VARCHAR(255),
primary key (EIB_OBJECT_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (MASK_ID)
      references MASK (MASK_ID)
) ENGINE = INNODB;

create table PRODUCT_DESCRIPTION (
PRODUCT_DESCRIPTION_ID INTEGER                        not null,
CATALOG_ENTRY_ID     INTEGER                        not null,
PRODUCT_DESCRIPTION_TEXT VARCHAR(1000),
DISPLAY_ORDER        SMALLINT,
LANGUAGE_ID          INTEGER,
primary key (PRODUCT_DESCRIPTION_ID),
foreign key (CATALOG_ENTRY_ID)
      references CATALOG_ENTRY (CATALOG_ENTRY_ID),
foreign key (LANGUAGE_ID)
      references ETE_LANGUAGE (LANGUAGE_ID)
) ENGINE = INNODB;

create table TEXT_TO_CATENT (
TEXT_TO_CATENT_ID    NUMERIC(6)                     not null,
CATALOG_ENTRY_ID     INTEGER                        not null,
SPEC_TEXT_ID         INTEGER                        not null,
primary key (TEXT_TO_CATENT_ID),
foreign key (CATALOG_ENTRY_ID)
      references CATALOG_ENTRY (CATALOG_ENTRY_ID),
foreign key (SPEC_TEXT_ID)
      references SPECIFICATION_TEXT (SPEC_TEXT_ID)
) ENGINE = INNODB;

create table EIB_PROPERTY (
EIB_PROPERTY_ID      INTEGER                        not null,
EIB_OBJECT_ID        INTEGER                        not null,
EIB_PROPERTY_NUMBER  SMALLINT,
WRITE_SWITCH         SMALLINT,
ARRAY_SWITCH         SMALLINT,
POINTER_SWITCH       SMALLINT,
PROPERTY_TYPE        SMALLINT,
PROPERTY_VALUE       INTEGER,
PROPERTY_DESCRIPTION VARCHAR(255),
primary key (EIB_PROPERTY_ID),
foreign key (EIB_OBJECT_ID)
      references EIB_OBJECT (EIB_OBJECT_ID)
) ENGINE = INNODB;

create table DUTY_CYCLE_CONDITION (
DUTY_CYCLE_CONDITION_ID INTEGER                        not null,
PROD2PROG_ID         INTEGER                        not null,
PARAMETER_ID         INTEGER                        not null,
PAR_PARM_VALUE       INTEGER,
DCY_F_INDIVIDUAL     FLOAT,
DCY_G_INDIVIDUAL     FLOAT,
DCY_F_GROUP          FLOAT,
DCY_G_GROUP          FLOAT,
primary key (DUTY_CYCLE_CONDITION_ID),
foreign key (PARAMETER_ID)
      references PARAMETER (PARAMETER_ID),
foreign key (PROD2PROG_ID)
      references PRODUCT_TO_PROGRAM (PROD2PROG_ID)
) ENGINE = INNODB;

create table LINE (
LINE_ID              INTEGER                        not null,
AREA_ID              INTEGER,
LINE_NAME            VARCHAR(50),
LINE_ADDRESS         INTEGER,
LINE_DESCRIPTION     VARCHAR(80),
LINE_FILTER_MODIFIED SMALLINT,
LINE_LOCKED          INTEGER,
LINE_UNIQUE_NAME     VARCHAR(50),
LAST_DEVICE_ADDRESS  SMALLINT,
IS_POWERLINE         SMALLINT,
MEDIUM_TYPE_NUMBER   SMALLINT,
MEDIUM_CHANNEL_ID    INTEGER,
CHANNEL_LIST_ID      INTEGER,
SYSTEM_ID            SMALLINT,
SYSTEM_ID_SCAN       SMALLINT,
REPEATER_MODE        SMALLINT,
primary key (LINE_ID),
foreign key (AREA_ID)
      references AREA (AREA_ID),
foreign key (CHANNEL_LIST_ID)
      references CHANNEL_LIST (CHANNEL_LIST_ID),
foreign key (MEDIUM_CHANNEL_ID)
      references MEDIUM_CHANNEL (MEDIUM_CHANNEL_ID),
foreign key (MEDIUM_TYPE_NUMBER)
      references MEDIUM_TYPE (MEDIUM_TYPE_NUMBER)
) ENGINE = INNODB;

create table DEVICE_ACCESSORY (
DEVICE_ACCESSORY_ID  INTEGER                        not null,
DEVICE_ID            INTEGER                        not null,
CATALOG_ENTRY_ID     INTEGER,
ACCESSORY_DESCRIPTION VARCHAR(80),
ACC_QUANTITY         INTEGER,
primary key (DEVICE_ACCESSORY_ID),
foreign key (CATALOG_ENTRY_ID)
      references CATALOG_ENTRY (CATALOG_ENTRY_ID)
) ENGINE = INNODB;

create table POLLING_GROUP (
POLLING_GROUP_ID     INTEGER                        not null,
LINE_ID              INTEGER                        not null,
POLLING_GROUP_NAME   VARCHAR(50),
POLLING_GROUP_NUMBER SMALLINT,
POLLING_GROUP_ADDRESS INTEGER,
POLLING_GROUP_DESCRIPTION VARCHAR(80),
primary key (POLLING_GROUP_ID),
foreign key (LINE_ID)
      references LINE (LINE_ID)
) ENGINE = INNODB;

create table DEVICE_PARAMETER (
DEVICE_ID            INTEGER                        not null,
DEVICE_PARAMETER_ID  INTEGER                        not null,
PARAMETER_ID         INTEGER,
DEVICE_PARAMETER_NUMBER SMALLINT,
DEVICE_PARAMETER_VISIBLE SMALLINT,
PARAMETER_VALUE_LONG INTEGER,
PARAMETER_VALUE_STRING VARCHAR(255),
PROGRAM_TYPE         SMALLINT,
PARAMETER_VALUE_DOUBLE DOUBLE PRECISION,
primary key (DEVICE_PARAMETER_ID),
foreign key (PARAMETER_ID)
      references PARAMETER (PARAMETER_ID)
) ENGINE = INNODB;

create table DEVICE (
DEVICE_ID            INTEGER                        not null,
LINE_ID              INTEGER,
ROOM_ID              INTEGER,
PROJECT_ID           INTEGER,
CATALOG_ENTRY_ID     INTEGER,
PROGRAM_ID           INTEGER,
FUNCTION_ID          INTEGER,
DEVICE_ADDRESS       INTEGER,
DEVICE_DESCRIPTION   VARCHAR(80),
DEVICE_DISPLAY_ORDER INTEGER,
DEVICE_DISPLAY_TEXT  VARCHAR(50),
DEVICE_LOCKED        INTEGER,
DEVICE_UNIQUE_NAME   VARCHAR(50),
DEVICE_BCU_PASSWORD  INTEGER,
DESIGNATION_NUMBER   INTEGER,
PEI_PROGRAM_ID       INTEGER,
DEVICE_BAU_TYPE      SMALLINT,
BAU_SERIAL_NUMBER_HIGH INTEGER,
BAU_SERIAL_NUMBER_LOW INTEGER,
SYMBOL_ID            INTEGER,
REPEATER_MODE        SMALLINT,
PRODUCT_HANDLING     SMALLINT,
POLLING_MASTER_TEXT  VARCHAR(255),
POLLING_SLAVE_TEXT   VARCHAR(255),
IS_DUMMY             SMALLINT,
DUTY_CYCLE_INDIVIDUAL FLOAT,
DUTY_CYCLE_GROUP     FLOAT,
CHANNEL_LIST_ID      INTEGER,
RE_NUMBER            SMALLINT,
DEVICE_LONG_TEXT     LONG VARCHAR,
ACTUAL_LENGTH        DOUBLE PRECISION,
primary key (DEVICE_ID),
foreign key (PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (PEI_PROGRAM_ID)
      references APPLICATION_PROGRAM (PROGRAM_ID),
foreign key (CATALOG_ENTRY_ID)
      references CATALOG_ENTRY (CATALOG_ENTRY_ID),
foreign key (CHANNEL_LIST_ID)
      references CHANNEL_LIST (CHANNEL_LIST_ID),
foreign key (FUNCTION_ID)
      references FUNCTION (FUNCTION_ID),
foreign key (LINE_ID)
      references LINE (LINE_ID),
foreign key (PROJECT_ID)
      references PROJECT (PROJECT_ID),
foreign key (ROOM_ID)
      references ROOM (ROOM_ID),
foreign key (SYMBOL_ID)
      references SYMBOL (SYMBOL_ID)
) ENGINE = INNODB;

create table DEVICE_TO_POLLING_GROUP (
DEVICE_TO_POLLING_GROUP_ID INTEGER                        not null,
POLLING_GROUP_ID     INTEGER                        not null,
DEVICE_ID            INTEGER                        not null,
IS_MASTER            SMALLINT,
SLAVE_NUMBER         SMALLINT,
primary key (DEVICE_TO_POLLING_GROUP_ID),
foreign key (DEVICE_ID)
      references DEVICE (DEVICE_ID),
foreign key (POLLING_GROUP_ID)
      references POLLING_GROUP (POLLING_GROUP_ID)
) ENGINE = INNODB;

create table SUBGROUP_TO_LINE2 (
LINE_ID              INTEGER                        not null,
SUBGROUP_ID          INTEGER                        not null,
primary key (LINE_ID, SUBGROUP_ID),
foreign key (LINE_ID)
      references LINE (LINE_ID),
foreign key (SUBGROUP_ID)
      references SUBGROUP (SUBGROUP_ID)
) ENGINE = INNODB;

create table DEVICE_TO_SPECIFICATION (
DEVICE_TO_SPEC_ID    INTEGER                        not null,
SPECIFICATION_ID     INTEGER,
DEVICE_ID            INTEGER,
SPEC_TEXT_ID         INTEGER,
DISPLAY_ORDER        SMALLINT,
primary key (DEVICE_TO_SPEC_ID),
foreign key (DEVICE_ID)
      references DEVICE (DEVICE_ID),
foreign key (SPECIFICATION_ID)
      references SPECIFICATION (SPECIFICATION_ID),
foreign key (SPEC_TEXT_ID)
      references SPECIFICATION_TEXT (SPEC_TEXT_ID)
) ENGINE = INNODB;

create table DEVICE_PROGRAMMING (
DEVICE_ID            INTEGER                        not null,
PROGRAMMING_STEP     INTEGER,
PHYSICAL_ADDRESS     SMALLINT,
APPLICATION_PROGRAM  SMALLINT,
PARAMETERS           SMALLINT,
COMMUNICATION_PART   SMALLINT,
LAST_MODIFIED        TIMESTAMP,
LAST_DOWNLOAD        TIMESTAMP,
DOWNLOADED_IMAGE     LONG VARBINARY,
DOWNLOADED_IMAGE_SIZE INTEGER,
REPEATER_FLAG        SMALLINT,
CHANNEL_NUMBER       SMALLINT,
ACTUAL_MASK_ID       INTEGER,
ACTUAL_CHANNEL_ID    INTEGER,
ACTUAL_RE_NUMBER     SMALLINT,
primary key (DEVICE_ID),
foreign key (DEVICE_ID)
      references DEVICE (DEVICE_ID),
foreign key (ACTUAL_MASK_ID)
      references MASK (MASK_ID),
foreign key (ACTUAL_CHANNEL_ID)
      references MEDIUM_CHANNEL (MEDIUM_CHANNEL_ID)
) ENGINE = INNODB;

create table DEVICE_EXTDATA (
DEVICE_ID            INTEGER                        not null,
DEVICE_DATA_CREATOR  VARCHAR(255),
DEVICE_DATA          LONG VARBINARY,
primary key (DEVICE_ID),
foreign key (DEVICE_ID)
      references DEVICE (DEVICE_ID)
) ENGINE = INNODB;

create table SUBGROUP_TO_OBJECT (
SUBGROUP_TO_OBJECT_ID INTEGER                        not null,
SUBGROUP_ID          INTEGER,
DEVICE_OBJECT_ID     INTEGER,
SEND                 SMALLINT,
ACKNOWLEDGE          SMALLINT,
primary key (SUBGROUP_TO_OBJECT_ID),
foreign key (DEVICE_OBJECT_ID)
      references DEVICE_OBJECT (DEVICE_OBJECT_ID),
foreign key (SUBGROUP_ID)
      references SUBGROUP (SUBGROUP_ID)
) ENGINE = INNODB;

create table DEVICE_TO_TENDER (
DEVICE_TO_TENDER_ID  INTEGER                        not null,
TENDER_ID            INTEGER,
DEVICE_ID            INTEGER,
SPEC_TEXT_ID         INTEGER,
DISPLAY_ORDER        SMALLINT,
PRICE                NUMERIC(12,3),
MATERIAL_PRICE       NUMERIC(12,3),
PERCENTAGE           NUMERIC(5,2),
MOUNTING_HOURS       SMALLINT,
MOUNTING_MINUTES     SMALLINT,
MOUNTING_SECONDS     SMALLINT,
SUM_DEVICE           NUMERIC(12,3),
SUM_MOUNTING         NUMERIC(12,3),
NUMBER_OF_DEVICES    INTEGER,
primary key (DEVICE_TO_TENDER_ID),
foreign key (DEVICE_ID)
      references DEVICE (DEVICE_ID),
foreign key (SPEC_TEXT_ID)
      references SPECIFICATION_TEXT (SPEC_TEXT_ID),
foreign key (TENDER_ID)
      references TENDER (TENDER_ID)
) ENGINE = INNODB;

create table LINE_FILTER_TABLE (
LINE_ID              INTEGER,
SUBGROUP_ID          INTEGER                        not null,
MANUAL               SMALLINT,
LINE_FILTER_TABLE_ID NUMERIC(6)                     not null,
AREA_ID              INTEGER,
primary key (LINE_FILTER_TABLE_ID),
foreign key (AREA_ID)
      references AREA (AREA_ID),
foreign key (LINE_ID)
      references LINE (LINE_ID),
foreign key (SUBGROUP_ID)
      references SUBGROUP (SUBGROUP_ID)
) ENGINE = INNODB;

create table VD_TO_DIST (
VD_TO_DIST_ID        NUMERIC(6)                     not null,
DISTRIBUTION_LIST_ID INTEGER                        not null,
VIRTUAL_DEVICE_ID    INTEGER                        not null,
primary key (VD_TO_DIST_ID),
foreign key (DISTRIBUTION_LIST_ID)
      references DISTRIBUTION_LIST (DISTRIBUTION_LIST_ID),
foreign key (VIRTUAL_DEVICE_ID)
      references VIRTUAL_DEVICE (VIRTUAL_DEVICE_ID)
) ENGINE = INNODB;

create unique index HELP_FILE_ALL on HELP_FILE (
ENTITY_ID DESC,
LANGUAGE_ID DESC
);

create  index HELP_FILE_ENTITY on HELP_FILE (
ENTITY_ID DESC
);

create unique index IX1_CHANNEL_LIST on CHANNEL_LIST (
MEDIUM_TYPE_NUMBER DESC,
CHANNEL_LIST_NAME DESC
);

create unique index IX1_CHANNEL_LIST_TO_CHANNEL on CHANNEL_LIST_TO_CHANNEL (
CHANNEL_LIST_ID DESC,
MEDIUM_CHANNEL_ID DESC
);

create unique index IX1_DUTY_CYCLE_CONDITION on DUTY_CYCLE_CONDITION (
PROD2PROG_ID DESC,
PARAMETER_ID DESC,
PAR_PARM_VALUE DESC
);

create unique index IX1_MASK_FEATURE on MASK_FEATURE (
MASK_FEATURE_NAME DESC
);

create unique index IX1_MASK_TO_MASK_FEATURE on MASK_TO_MASK_FEATURE (
MASK_ID DESC,
MASK_FEATURE_ID DESC
);

create unique index IX1_MEDIUM_CHANNEL on MEDIUM_CHANNEL (
MEDIUM_TYPE_NUMBER DESC,
CHANNEL_NUMBER DESC
);

create unique index IX1_PRODUCT_ATTR on PRODUCT_ATTR (
HW_PRODUCT_ID DESC,
ATTR_ID DESC
);

create unique index IX1_PROGRAM_PLUGIN on PROGRAM_PLUGIN (
PROGRAM_ID DESC,
PLUGIN_TYPE DESC
);

create unique index IX1_PROGRAM_TO_MASK_FEATURE on PROGRAM_TO_MASK_FEATURE (
PROGRAM_ID DESC,
MASK_FEATURE_ID DESC
);

create  index TEXT_ATTRIBUTE_ENTITY on TEXT_ATTRIBUTE (
ENTITY_ID DESC
);

create unique index TEXT_ATTRIBUTE_I1 on TEXT_ATTRIBUTE (
COLUMN_ID DESC,
ENTITY_ID DESC,
LANGUAGE_ID DESC
);

create  index device_accessory_device_id on DEVICE_ACCESSORY (
DEVICE_ID DESC
);

create  index device_object_device_id on DEVICE_OBJECT (
DEVICE_ID DESC
);

create  index device_parameter_device_id on DEVICE_PARAMETER (
DEVICE_ID DESC
);

create  index maingroup_project_id on MAINGROUP (
PROJECT_ID DESC
);

create view Accessories as
select ACCESSORY_ID as ID, ACC_QUANTITY as Quantity, CATALOG_ENTRY_ID as CatalogEntryID, CAT_CATALOG_ENTRY_ID as ParentCatalogEntryID
from ACCESSORY;

create view AddressFixups as
select FIXUP_ID as ID, FIXUP_NAME as "Name", FIXUP_ADDRESS as "Address", PROGRAM_ID as ApplicationProgramID, FIXUP_TYPE as FixupType
from ADDRESS_FIXUP;

create view ApplicationPrograms as
select PROGRAM_ID as ID, MANUFACTURER_ID as ManufacturerID, ORIGINAL_MANUFACTURER_ID as OriginalManufacturerID, PROGRAM_NAME as "Name", PROGRAM_VERSION as Version, LINKABLE as Linkable, DEVICE_TYPE as DeviceType, PEI_TYPE as PEIType, EEPROM_DATA as EEPROMData, PROGRAM_TYPE as "Type", RAM_SIZE as RequiredRAMSize, IS_POLLING_MASTER as IsPollingMaster, NUMBER_OF_POLLING_GROUPS as NumberOfPollingGroups, SYMBOL_ID as SymbolID, MASK_ID as MaskID, ADDRESS_TAB_SIZE as AddressTabSize, ASSOCTAB_SIZE as AssocTabSize, DYNAMIC_MANAGEMENT as DynamicManagement
from APPLICATION_PROGRAM;

create view Areas as
select AREA_ID as ID, AREA_NAME as "Name", AREA_UNIQUE_NAME as UniqueName, AREA_DESCRIPTION as Description, AREA_ADDRESS as "Address", AREA_LOCKED as LockType, PROJECT_ID as ProjectID
from AREA;

create view AssociationTemplates as
select SubGroup_To_Object_ID as ID,
    "Send" as "Send",
    Acknowledge as Acknowledge,
    Device_Object_ID as DeviceTemplateCommunicationObjectID,
    Subgroup_ID as GroupAddressTemplateID
    from SubGroup_To_Object
    where Device_Object_ID in (select Device_Object_ID from Device_Object
      where Device_ID in (select Virtual_Device_ID from Virtual_Device));

create view Associations as
select SUBGROUP_TO_OBJECT_ID as ID, "Send" as "Send", ACKNOWLEDGE as Acknowledge, DEVICE_OBJECT_ID as DeviceCommunicationObjectID, SUBGROUP_ID as GroupAddressID
from SUBGROUP_TO_OBJECT;

create view BCUTypes as
select BCU_TYPE_NUMBER as ID, BCU_TYPE_NAME as "Name", BCU_TYPE_CPU as CPU
from BCU_TYPE;

create view Buildings as
select BUILDING_ID as ID, BUILDING_NAME as "Name", BUILDING_UNIQUE_NAME as UniqueName, BUILDING_NUMB as Number, BUILDING_DESCRIPTION as Description, BUILDING_LOCKED as Locked, BUI_BUILDING_ID as ParentID, PROJECT_ID as ProjectID
from BUILDING;

create view CatalogEntries as
select CATALOG_ENTRY_ID as ID, ORDER_NUMBER as OrderNumber, ENTRY_NAME as "Name", DIN_FLAG as RailMounted, RAM_SIZE as RAMSize, PRODUCT_ID as HardwareProductID, MANUFACTURER_ID as ManufacturerID, SYMBOL_ID as SymbolID, ENTRY_COLOUR as Colour, ENTRY_WIDTH_IN_MODULES as WidthInModules, ENTRY_WIDTH_IN_MILLIMETERS as WidthInMillimeters, QUANTITY_UNIT as QuantityUnit, SERIES as Series, CATALOG_NAME as CatalogName, PAGE_NUMBER as PageNumber, ENTRY_PICTURE as EntryPicture, REGISTRATION_NUMBER as RegistrationNumber, REGISTRATION_YEAR as RegistrationYear, ENTRY_STATUS_CODE as EntryStatusCode, REGISTRATION_TS as RegistrationTS, REGISTRATION_DATE as RegistrationDate, REGISTRATION_COMMENT as RegistrationComment
from CATALOG_ENTRY;

create view CommunicationObjects as
select OBJECT_ID as ID, OBJECT_NAME as "Name", OBJECT_FUNCTION as "Function", OBJECT_READENABLED as ReadEnabled, OBJECT_WRITEENABLED as WriteEnabled, OBJECT_COMMENABLED as CommunicationEnabled, OBJECT_TRANSENABLED as TransmissionEnabled, PARENT_PARAMETER_VALUE as ParentParameterValue, OBJECT_NUMBER as Number, OBJECT_DESCRIPTION as Description, OBJECT_TYPE as Length, OBJECT_PRIORITY as TransmissionPriority, OBJECT_UPDATEENABLED as UpdateEnabled, OBJECT_UNIQUE_NUMBER as UniqueNumber, PROGRAM_ID as ApplicationProgramID, PARAMETER_ID as ParentParameterID
from COMMUNICATION_OBJECT;

create view _Database as
select
(conv(Major_Version_Number,10,10) + '.' + conv(Minor_Version_Number,10,10)) as Version,
USER_TYPE as UserType, DATABASE_ID as ID
from ETS_DATABASE_VERSION;

create view DeviceAccessories as select Device_Accessory_ID as ID,
    ACC_Quantity as Quantity,
    Device_ID as DeviceID,
    Catalog_Entry_ID as CatalogEntryID,
    Accessory_Description as Description
    from Device_Accessory
    where (Device_ID=(select distinct Device_ID from Device));

create view DeviceCommunicationObjects as
select Device_Object_ID as ID,
    Device_Object_Unique_Name as UniqueName,
    Device_Object_Visible as Visible,
    Object_Prio as TransmissionPriority,
    Object_Read as ReadEnabled,
    Object_Write as WriteEnabled,
    Object_Comm as CommunicationEnabled,
    Object_Trans as TransmissionEnabled,
    Object_Update as UpdateEnabled,
    Device_ID as DeviceID,
    Object_ID as CommunicationObjectID,
    (select Eib_Data_Type_Name from Eib_Data_Type
      where Eib_Data_Type_Code=Device_Object.Eib_Data_Type_Code) as EibDataTypeName,
    (select Eis_Type_Name from Eib_Data_Type
      where Eib_Data_Type_Code=Device_Object.Eib_Data_Type_Code) as EisTypeName,
    (select Eib_Data_Subtype_Name from Eib_Data_Subtype
      where(Eib_Data_Type_Code=Device_Object.Eib_Data_Type_Code
      and Eib_Data_Subtype_Code=Device_Object.Eib_Data_Subtype_Code)) as EibDataSubtypeName,
    Eib_Data_Type_Code as EibDataTypeID,
    Eib_Data_Subtype_Code as EibDataSubtypeID
    from Device_Object;

create view DeviceExtDatas as
select DEVICE_ID as ID, DEVICE_DATA_CREATOR as DeviceDataCreator, DEVICE_DATA as DeviceData
from DEVICE_EXTDATA;

create view DeviceParameters as
select DEVICE_PARAMETER_ID as ID, DEVICE_PARAMETER_VISIBLE as Visible, DEVICE_ID as DeviceID, PARAMETER_ID as ParameterID, PARAMETER_VALUE_DOUBLE as ValueDouble, PARAMETER_VALUE_LONG as ValueLong, PARAMETER_VALUE_STRING as ValueString
from DEVICE_PARAMETER;

create view DevicePollingGroups as
select DEVICE_TO_POLLING_GROUP_ID as ID, IS_MASTER as IsMaster, SLAVE_NUMBER as SlaveNumber, POLLING_GROUP_ID as PollingGroupID, DEVICE_ID as DeviceID
from DEVICE_TO_POLLING_GROUP;

create view DeviceTemplateAccessories as
select Device_Accessory_ID as ID,
    ACC_Quantity as Quantity,
    Device_ID as DeviceTemplateID,
    Catalog_Entry_ID as CatalogEntryID,
    Accessory_Description as Description
    from Device_Accessory
    where (Device_ID=(select distinct Virtual_Device_ID from Virtual_Device));

create view DeviceTemplateCommObjects as
select Device_Object_ID as ID,
    Device_Object_Type as "Type",
    Device_Object_Visible as Visible,
    Object_Prio as TransmissionPriority,
    Object_Read as ReadEnabled,
    Object_Write as WriteEnabled,
    Object_Comm as CommunicationEnabled,
    Object_Trans as TransmissionEnabled,
    Object_Update as UpdateEnabled,
    Device_ID as DeviceTemplateID,
    Object_ID as CommunicationObjectID
    from Device_Object
    where Device_ID=(select distinct Virtual_Device_ID from Virtual_Device);

create view DeviceTemplateParameters as
select Device_Parameter_ID as ID,
    Device_Parameter_Visible as Visible,
    Device_ID as DeviceTemplateID,
    Parameter_ID as ParameterID,
    Parameter_Value_Double as ValueDouble,
    Parameter_Value_Long as ValueLong,
    Parameter_Value_String as ValueString
    from Device_Parameter
    where Device_ID=(select distinct Virtual_Device_ID from Virtual_Device);

create view DeviceTemplates as
select VIRTUAL_DEVICE_ID as ID, VIRTUAL_DEVICE_NAME as "Name", VIRTUAL_DEVICE_NUMBER as Number, VIRTUAL_DEVICE_DESCRIPTION as Description, CATALOG_ENTRY_ID as CatalogEntryID, FUNCTIONAL_ENTITY_ID as FunctionalEntityID, PROGRAM_ID as ApplicationProgramID, SYMBOL_ID as SymbolID, PEI_PROGRAM_ID as PEIProgramID
from VIRTUAL_DEVICE;

create view Devices as
select Device_ID as ID,
    Device_Unique_Name as UniqueName,
    Device_Description as Description,
    Device_Address as "Address",
    Device_Locked as LockType,
    Device_BCU_Password as BCUKey,
    Designation_Number as DesignationNumber,
    Project_ID as ProjectID,
    Line_ID as LineID,
    Symbol_ID as SymbolID,
    Catalog_Entry_ID as CatalogEntryID,
    Program_ID as ApplicationProgramID,
    Room_ID as RoomID,
    Function_ID as FunctionID,
    Device_BAU_Type as BAUType,
    BAU_Serial_Number_Low as BAUSerialNumberLow,
    BAU_Serial_Number_High as BAUSerialNumberHigh,
    (select Line_Address from Line where Line.Line_ID=Device.Line_ID) as LineAddress,
    (select Area_Address from Area where Area.Area_ID
      =(select Line.Area_ID from Line where Line.Line_ID=Device.Line_ID)) as AreaAddress,
    PEI_Program_ID as PEIProgramID,
    Device_Long_Text as InstallationHints,
    RE_Number as ReNumber,
    if (left(Device_Long_Text,5)='{\\rtf','rtf','text') as InstallationHintsFormat
    from Device;


create view DistributionLists as
select DISTRIBUTION_LIST_ID as ID, DISTRIBUTION_LIST_NAME as "Name", DISTRIBUTION_LIST_NUMB as Number, MANUFACTURER_ID as ManufacturerID, DISTRIBUTION_LIST_DESCRIPTION as Description
from DISTRIBUTION_LIST;

create view DownloadParameters as
select Parameter_ID as ID,
    Parameter_Number as UniqueNumber,
    Parameter_Name as "Name",
    Parameter_Low_Access as LowAccess,
    Parameter_High_Access as HighAccess,
    Parent_Parm_Value as ParentParameterValue,
    Parameter_Function as "Function",
    Parameter_Display_Order as DisplayOrder,
    Parameter_Address as "Address",
    Parameter_BitOffset as BitOffset,
    Parameter_Description as Description,
    Parameter_Default_Long as DefaultLong,
    Parameter_Default_Double as DefaultDouble,
    Parameter_Default_String as DefaultString,
    Program_ID as ApplicationProgramID,
    Par_Parameter_ID as ParentParameterID,
    Parameter_Type_ID as ParameterTypeID,
    Patch_Always as PatchAlways,
    Address_Space as AddressSpace,
    Eib_Object_Ref as EibObjectRef,
    Eib_Property_ID as EibPropertyID
    from Parameter
    where Address_Space>=2;

create view EibDataSubtypes as
select EIB_DATA_SUBTYPE_CODE as ID, EIB_DATA_SUBTYPE_NAME as "Name", EIB_DATA_TYPE_CODE as EibDataTypeID, EIB_DATA_SUBTYPE_TEXT as Text, ALLOWED_FOR_COMM_OBJECTS as AllowedForCommObjects
from EIB_DATA_SUBTYPE;

create view EibDataTypes as
select EIB_DATA_TYPE_CODE as ID, EIB_DATA_TYPE_NAME as "Name", LENGTH_IN_BIT as LengthInBit, EIS_TYPE_NAME as EisTypeName
from EIB_DATA_TYPE;

create view FunctionalEntities as
select FUNCTIONAL_ENTITY_ID as ID, FUNCTIONAL_ENTITY_NAME as "Name", FUNCTIONAL_ENTITY_NUMB as Number, FUN_FUNCTIONAL_ENTITY_ID as ParentID, FUNCTIONAL_ENTITY_DESCRIPTION as Description, MANUFACTURER_ID as ManufacturerID
from FUNCTIONAL_ENTITY;

create view Functions as
select Function_ID as ID,
    Function_Name as "Name",
    Function_Unique_Name as UniqueName,
    Function_Numb as Number,
    Function_Description as Description,
    Function_Locked as Locked,
    Fun_Function_ID as ParentID,
    Project_ID as ProjectID
    from Function;

create view GroupAddressTemplates as
select SubGroup_ID as ID,
    SubGroup_Name as "Name",
    SubGroup_Description as Description,
    SubGroup_Address as "Address",
    SubGroup_Type as Length,
    SubGroup_Central as Central,
    MiddleGroup_ID as MiddleGroupTemplateID,
    (select MiddleGroup_Address from MiddleGroup
      where MiddleGroup.MiddleGroup_ID=SubGroup.MiddleGroup_ID) as MiddleGroupTemplateAddress,
    (select MainGroup_Address from MainGroup
      where MainGroup.MainGroup_ID=(select MiddleGroup.MainGroup_ID from MiddleGroup
        where MiddleGroup.MiddleGroup_ID=SubGroup.MiddleGroup_ID)) as MainGroupTemplateAddress
    from SubGroup
    where(MiddleGroup_ID=(select distinct MiddleGroup_ID from MiddleGroup
      where(MainGroup_ID=(select distinct MainGroup_ID from MainGroup
        where(Project_ID=(select distinct Functional_Entity_ID from Functional_Entity))))));

create view GroupAddresses as
select SubGroup_ID as ID,
    SubGroup_Name as "Name",
    SubGroup_Unique_Name as UniqueName,
    SubGroup_Description as Description,
    SubGroup_Address as "Address",
    SubGroup_Passthrough as "PassThrough",
    SubGroup_Locked as LockType,
    SubGroup_Type as Length,
    SubGroup_Central as Central,
    MiddleGroup_ID as MiddleGroupID,
    (select MiddleGroup_Address from MiddleGroup
      where MiddleGroup.MiddleGroup_ID=SubGroup.MiddleGroup_ID) as MiddleGroupAddress,
    (select MainGroup_Address from MainGroup
      where MainGroup.MainGroup_ID=(select MiddleGroup.MainGroup_ID from MiddleGroup
        where MiddleGroup.MiddleGroup_ID=SubGroup.MiddleGroup_ID)) as MainGroupAddress
    from SubGroup;

create view HardwareProducts as
select PRODUCT_ID as ID, PRODUCT_NAME as "Name", PRODUCT_VERSION_NUMBER as VersionNumber, COMPONENT_ATTRIBUTES as ComponentAttributes, BUS_CURRENT as BusCurrent, PRODUCT_SERIAL_NUMBER as SerialNumber, PRODUCT_PICTURE as Picture, BCU_TYPE_NUMBER as BCUTypeNumber, MANUFACTURER_ID as ManufacturerID, ORIGINAL_MANUFACTURER_ID as OriginalManufacturerID, SYMBOL_ID as SymbolID
from HW_PRODUCT;

create view Languages as
select LANGUAGE_ID as ID, LANGUAGE_NAME as "Name", DATABASE_LANGUAGE as DatabaseLanguage
from ETE_LANGUAGE;

create view _Lines as
select LINE_ID as ID, LINE_NAME as "Name", LINE_UNIQUE_NAME as UniqueName, LINE_DESCRIPTION as Description, LINE_ADDRESS as "Address", LINE_FILTER_MODIFIED as FilterModified, LINE_LOCKED as LockType, SYSTEM_ID_SCAN as DomainAddressScanned, SYSTEM_ID as DomainAddress, AREA_ID as AreaID, MEDIUM_TYPE_NUMBER as MediumTypeNumber, MEDIUM_CHANNEL_ID as MediumChannelID, CHANNEL_LIST_ID as ChannelListID, (select Area_Address from Area where Area.Area_ID=Line.Area_ID) as AreaAddress
from LINE;

create view MainGroupTemplates as
select MainGroup_ID as ID,
    MainGroup_Name as "Name",
    MainGroup_Description as Description,
    MainGroup_Address as "Address",
    Project_ID as FunctionalEntityID
    from MainGroup
    where Project_ID=(select distinct Functional_Entity_ID from Functional_Entity);

create view MainGroups as
select MAINGROUP_ID as ID, MAINGROUP_NAME as "Name", MAINGROUP_UNIQUE_NAME as UniqueName, MAINGROUP_DESCRIPTION as Description, MAINGROUP_ADDRESS as "Address", MAINGROUP_LOCKED as LockType, MAINGROUP_PASSTHROUGH as "PassThrough", PROJECT_ID as ProjectID
from MAINGROUP;

create view ManufacturerCompatibilities as
select MANUFACTURER_COMPATIBILITY_ID as ID, MANUFACTURER_ID as ManufacturerID, MAN_MANUFACTURER_ID as CompatibleManufacturerID
from MANUFACTURER_COMPATIBILITY;

create view Manufacturers as
select MANUFACTURER_ID as ID, MANUFACTURER_NAME as "Name"
from MANUFACTURER;

create view MaskCompatibilities as
select MASK_COMPATIBILITY_ID as ID, OLD_MASK_ID as OldMaskID, NEW_MASK_ID as NewMaskID, STATUS as Status
from MASK_COMPATIBILITY;

create view MaskEntries as
select MASK_ENTRY_ID as ID, MASK_ENTRY_NAME as "Name", MASK_ENTRY_ADDRESS as "Address", MASK_ID as MaskID
from MASK_ENTRY;

create view MaskFeatures as
select MASK_FEATURE_ID as ID, MASK_FEATURE_NAME as "Name"
from MASK_FEATURE;

create view MaskToMaskFeatures as
select MASK_TO_MASK_FEATURE_ID as ID, MASK_ID as MaskID, MASK_FEATURE_ID as MaskFeatureID, MASK_FEATURE_VALUE as MaskFeatureValue
from MASK_TO_MASK_FEATURE;

create view Masks as
select MASK_ID as ID, MASK_VERSION as Version, MASK_VERSION_NAME as "Name", MEDIUM_TYPE_NUMBER as MediumTypeNumber, MEDIUM_TYPE_NUMBER2 as MediumTypeNumber2, BCU_TYPE_NUMBER as BCUTypeNumber
from MASK;

create view MediumChannels as
select MEDIUM_CHANNEL_ID as ID, CHANNEL_NUMBER as ChannelNumber, MAX_DEVICE_DUTY_CYCLE as MaxDeviceDutyCycle, MEDIUM_TYPE_NUMBER as MediumTypeID
from MEDIUM_CHANNEL;

create view MediumTypes as
select MEDIUM_TYPE_NUMBER as ID, MEDIUM_TYPE_NAME as "Name", MEDIUM_TYPE_SHORT_NAME as ShortName, STANDARD_SYSTEM_ID_LEVEL as StandardSystemIDLevel, SYSTEM_ID_LENGTH as SystemIDLength, DCY_F_INDIVIDUAL as DCY_F_Individual, DCY_G_INDIVIDUAL as DCY_G_Individual, DCY_F_GROUP as DCY_F_Group, DCY_G_GROUP as DCY_G_Group, DUTY_CYCLE_CALCULATION as DutyCycleCalculation
from MEDIUM_TYPE;

create view MiddleGroupTemplates as
select MiddleGroup_ID as ID,
    MiddleGroup_Name as "Name",
    MiddleGroup_Description as Description,
    MiddleGroup_Address as "Address",
    MainGroup_ID as MainGroupTemplateID
    from MiddleGroup
    where(MainGroup_ID=(select distinct MainGroup_ID from MainGroup
      where(Project_ID=(select distinct Functional_Entity_ID from Functional_Entity))));

create view MiddleGroups as
select MIDDLEGROUP_ID as ID, MIDDLEGROUP_NAME as "Name", MIDDLEGROUP_UNIQUE_NAME as UniqueName, MIDDLEGROUP_DESCRIPTION as Description, MIDDLEGROUP_ADDRESS as "Address", MIDDLEGROUP_PASSTHROUGH as "PassThrough", MIDDLEGROUP_LOCKED as LockType, MAINGROUP_ID as MainGroupID
from MIDDLEGROUP;

create view ParameterEnumValues as
select PARAMETER_VALUE_ID as ID, REAL_VALUE as RealValue, DISPLAYED_VALUE as DisplayedValue, DISPLAY_ORDER as DisplayOrder, BINARY_VALUE as BinaryValue, PARAMETER_TYPE_ID as ParameterTypeID
from PARAMETER_LIST_OF_VALUES;

create view ParameterTypes as
select PARAMETER_TYPE_ID as ID, ATOMIC_TYPE_NUMBER as BaseType, PARAMETER_TYPE_NAME as "Name", PARAMETER_MINIMUM_VALUE as MinimumValue, PARAMETER_MAXIMUM_VALUE as MaximumValue, PARAMETER_TYPE_DESCRIPTION as Description, PARAMETER_TYPE_LOW_ACCESS as LowAccess, PARAMETER_TYPE_HIGH_ACCESS as HighAccess, PARAMETER_TYPE_SIZE as SizeInBits, PROGRAM_ID as ApplicationProgramID, PARAMETER_UI_HINT as ParameterUIHint
from PARAMETER_TYPE;

create view Parameters as
select PARAMETER_ID as ID, PARAMETER_NUMBER as UniqueNumber, PARAMETER_NAME as "Name", PARAMETER_LOW_ACCESS as LowAccess, PARAMETER_HIGH_ACCESS as HighAccess, PARENT_PARM_VALUE as ParentParameterValue, PARAMETER_FUNCTION as "Function", PARAMETER_DISPLAY_ORDER as DisplayOrder, PARAMETER_ADDRESS as "Address", PARAMETER_BITOFFSET as BitOffset, PARAMETER_DESCRIPTION as Description, PARAMETER_DEFAULT_LONG as DefaultLong, PARAMETER_DEFAULT_DOUBLE as DefaultDouble, PARAMETER_DEFAULT_STRING as DefaultString, PROGRAM_ID as ApplicationProgramID, PAR_PARAMETER_ID as ParentParameterID, PARAMETER_TYPE_ID as ParameterTypeID, PATCH_ALWAYS as PatchAlways, ADDRESS_SPACE as AddressSpace, EIB_OBJECT_REF as EibObjectRef, EIB_PROPERTY_ID as EibPropertyID
from PARAMETER;

create view PollingGroups as
select POLLING_GROUP_ID as ID, POLLING_GROUP_NAME as "Name", POLLING_GROUP_DESCRIPTION as Description, POLLING_GROUP_NUMBER as Number, POLLING_GROUP_ADDRESS as "Address", LINE_ID as LineID
from POLLING_GROUP;

create view ProductDescriptions as
select PRODUCT_DESCRIPTION_ID as ID, CATALOG_ENTRY_ID as CatalogEntryID, PRODUCT_DESCRIPTION_TEXT as Text, DISPLAY_ORDER as DisplayOrder, LANGUAGE_ID as LanguageID
from PRODUCT_DESCRIPTION;

create view ProductProgramMediumTypes as
select PROD2PROG2MT_ID as ID, MEDIUM_TYPE_NUMBER as MediumTypeNumber, PROD2PROG_ID as ProductProgramID, CHANNEL_LIST_ID as ChannelListID
from PRODUCT_TO_PROGRAM_TO_MT;

create view ProductPrograms as
select PROD2PROG_ID as ID, PRODUCT_ID as HardwareProductID, PROD2PROG_STATUS_CODE as StatusCode, REGISTRATION_NUMBER as RegistrationNumber, REGISTRATION_YEAR as RegistrationYear, ORIGINAL_REGISTRATION_NUMBER as OriginalRegistrationNumber, ORIGINAL_REGISTRATION_YEAR as OriginalRegistrationYear, EXPIRATION_DATE as ExpirationDate, REGISTRATION_TS as RegistrationTimeStamp, PROGRAM_ID as ApplicationProgramID, PEI_PROGRAM_ID as PEIProgramID, REGISTRATION_COMMENT as RegistrationComment, REGISTRATION_DATE as RegistrationDate, REGISTRATION_MEDIUM_TYPE as RegistrationMediumTypeID, DCY_F_INDIVIDUAL as DCY_F_Individual, DCY_G_INDIVIDUAL as DCY_G_Individual, DCY_F_GROUP as DCY_F_Group, DCY_G_GROUP as DCY_G_Group
from PRODUCT_TO_PROGRAM;

create view ProgramDescriptions as
select PROGRAM_DESCRIPTION_ID as ID, PROGRAM_ID as ApplicationProgramID, TEXT as Text, DISPLAY_ORDER as DisplayOrder, LANGUAGE_ID as LanguageID
from PROGRAM_DESCRIPTION;

create view ProgramPlugins as
select PROGRAM_PLUGIN_ID as ID, PROGRAM_ID as ApplicationProgramID, PLUGIN_TYPE as PluginType, PLUGIN_CLSID as PluginCLSID, PLUGIN_CODEBASE as PluginCodebase
from PROGRAM_PLUGIN;

create view ProgramToMaskFeatures as
select PROGRAM_TO_MASK_FEATURE_ID as ID, PROGRAM_ID as ProgramID, MASK_FEATURE_ID as MaskFeatureID, MASK_FEATURE_VALUE as MaskFeatureValue
from PROGRAM_TO_MASK_FEATURE;

create view ProjectHistories as
select PROJECT_HISTORY_ID as ID, PROJECT_HISTORY_DATE as "Date", PROJECT_HISTORY_TEXT as Text, PROJECT_ID as ProjectID
from PROJECT_HISTORY;

create view ProjectParts as
select Building_ID as ID,'Building' as PartType,
    Bui_Building_ID as ParentID,
    Building_Name as "Name",
    Building_Unique_Name as UniqueName,
    Building_Numb as Number,
    Building_Description as Description,
    Building_Locked as LockType,
    Project_ID as ProjectID
    from Building
  union select Room_ID as ID,'Room_'
    +Room_Outline as PartType,
    Building_ID as ParentID,
    Room_Name as "Name",
    Room_Unique_Name as UniqueName,
    Room_Numb as Number,
    Room_Description as Description,
    Room_Locked as LockType,
    Project_ID as ProjectID
    from Room
  union select Function_ID as ID,'Function' as PartType,
    Fun_Function_ID as ParentID,
    Function_Name as "Name",
    Function_Unique_Name as UniqueName,
    Function_Numb as Number,
    Function_Description as Description,
    Function_Locked as LockType,
    Project_ID as ProjectID
    from Function;

create view Projects as
select PROJECT_ID as ID, PROJECT_NAME as "Name", PROJECT_NUMB as Number, CONTRACT_NUMBER as ContractNumber, PROJECT_BCU_PASSWORD as StandardBCUKey, BEGIN_DATE as BeginDate, END_DATE as EndDate, CONVERSION_DATE as ConversionDate, IMPORT_DATE as ImportDate
from PROJECT;

create view Rooms as
select ROOM_ID as ID, ROOM_NAME as "Name", ROOM_UNIQUE_NAME as UniqueName, ROOM_NUMB as Number, ROOM_DESCRIPTION as Description, ROOM_LOCKED as Locked, ROOM_OUTLINE as Outline, BUILDING_ID as ParentID, PROJECT_ID as ProjectID
from ROOM;

create view Symbols as
select SYMBOL_ID as ID, SYMBOL_NAME as "Name", SYMBOL_FILENAME as Filename, SYMBOL_DATA as Data
from SYMBOL;

create view TextAttributes as
select TEXT_ATTRIBUTE_ID as ID, LANGUAGE_ID as LanguageID, COLUMN_ID as ColumnID, ENTITY_ID as EntityID, TEXT_ATTRIBUTE_TEXT as Text
from TEXT_ATTRIBUTE;

create view Translations as
select TEXT_ATTRIBUTE_ID as ID, LANGUAGE_ID as LanguageID, COLUMN_ID as ColumnID, ENTITY_ID as EntityID, TEXT_ATTRIBUTE_TEXT as Text
from TEXT_ATTRIBUTE;

create view UIParameters as
select Parameter_ID as ID,
    Parameter_Number as UniqueNumber,
    Parameter_Name as "Name",
    Parameter_Low_Access as LowAccess,
    Parameter_High_Access as HighAccess,
    Parent_Parm_Value as ParentParameterValue,
    Parameter_Function as "Function",
    Parameter_Display_Order as DisplayOrder,
    Parameter_Address as "Address",
    Parameter_BitOffset as BitOffset,
    Parameter_Description as Description,
    Parameter_Default_Long as DefaultLong,
    Parameter_Default_Double as DefaultDouble,
    Parameter_Default_String as DefaultString,
    Program_ID as ApplicationProgramID,
    Par_Parameter_ID as ParentParameterID,
    Parameter_Type_ID as ParameterTypeID,
    Patch_Always as PatchAlways,
    Address_Space as AddressSpace,
    Eib_Object_Ref as EibObjectRef,
    Eib_Property_ID as EibPropertyID
    from Parameter
    where Address_Space<2;

create view maingroup_view as
select project_id,maingroup.MAINGROUP_ID, count(middlegroup_id)
from maingroup join middlegroup
group by MAINGROUP_ID, project_id;

create view middlegroup_view as
select maingroup_id,middlegroup.MIDDLEGROUP_ID, count(subgroup_id)
from middlegroup join subgroup
group by MIDDLEGROUP_ID, maingroup_id;

#create view prefab_finder as
#select fe.functional_entity_name, fe.functional_entity_id, fe.functional_entity_description,
#man.manufacturer_name, man.manufacturer_id,
#( select first pf.functional_entity_name, from virtual_device as vd join functional_entity as pt
#on vd.product_type_id=pt.functional_entity_id join, functional_entity as pf,
#on pt.fun_functional_entity_id=pf.functional_entity_id,
#where vd.functional_entity_id=fe.functional_entity_id),
#( select first pt.functional_entity_name, from virtual_device as vd join functional_entity as pt
#on vd.product_type_id=pt.functional_entity_id,
#where vd.functional_entity_id=fe.functional_entity_id)
#from functional_entity as fe join manufacturer as man maingroup as mg
#where  exists ( select mg.PROJECT_ID , mg.MAINGROUP_ID ,
#mg.MAINGROUP_NAME , mg.MAINGROUP_ADDRESS , mg.MAINGROUP_PASSTHROUGH ,
#mg.MAINGROUP_DESCRIPTION , mg.MAINGROUP_LOCKED , mg.MAINGROUP_UNIQUE_NAME ,
#mg.LAST_MIDDLEGROUP_ADDRESS mg.project_id = fe.functional_entity_id));

#create view product_finder as
#select vd.virtual_device_name, vd.virtual_device_id, ce.order_number, ce.entry_name,
#ce.series, ap.program_name, pf.functional_entity_name, pt.functional_entity_name,
#man.manufacturer_name, man.manufacturer_id, vd.medium_types, ce.entry_colour
#from virtual_device as vd join catalog_entry as ce, virtual_device as vd
#left outer join application_program as ap on vd.program_id=ap.program_id,
#virtual_device as vd join functional_entity as pt on vd.product_type_id=pt.functional_entity_id
#join functional_entity as pf on pt.fun_functional_entity_id=pf.functional_entity_id
#join manufacturer as man);

#create view product_finder_old as
#select vd.virtual_device_id, ce.order_number, ta.text_attribute_text, tf.text_attribute_text, tb.text_attribute_text, td.text_attribute_text, tc.text_attribute_text, man.manufacturer_name, man.manufacturer_id
#from virtual_device as vd key join catalog_entry as ce left outer join text_attribute as ta on ce.catalog_entry_id=ta.entity_id and ta.column_id=1 key left outer join ete_language as ela on ela.database_language=1, catalog_entry as ce left outer join text_attribute as tf on ce.catalog_entry_id=tf.entity_id and tf.column_id=3 key left outer join ete_language as elf on elf.database_language=1, virtual_device as vd left outer join application_program as ap on vd.program_id=ap.program_id left outer join text_attribute as tb on ap.program_id=tb.entity_id and tb.column_id=80 key left outer join ete_language as elb on elb.database_language=1, virtual_device as vd join functional_entity as pt on vd.product_type_id=pt.functional_entity_id left outer join text_attribute as tc on pt.functional_entity_id=tc.entity_id and tc.column_id=30 key left outer join ete_language as elc on elc.database_language=1, functional_entity as pt
#left outer join functional_entity as pf on pt.fun_functional_entity_id=pf.functional_entity_id left outer join text_attribute as td on pf.functional_entity_id=td.entity_id and td.column_id=30 key left outer join ete_language as eld on eld.database_language=1, functional_entity as pf key join manufacturer as man;

create view quantity_structure as
select PROJECT_ID, CATALOG_ENTRY_ID, count(device_id)
from DEVICE
group by PROJECT_ID, CATALOG_ENTRY_ID;

#create view regtool_finder as
#select IFNULL(p2p.program_id, hwp.product_id, p2p.prod2prog_id), ce.catalog_entry_id, IFNULL(p2p.program_id, 2, 0), IFNULL(p2p.program_id, 'Product', 'Program' ), MOD((hwp.component_attributes/8), 2), m.manufacturer_id, m.manufacturer_name, ce.entry_name, ce.order_number, IFNULL(p2p.program_id, hwp.product_name, ap.program_name), ap.program_version, p2p.registration_number, p2p.prod2prog_status_code, p2p.original_registration_number, p2p.expiration_date, s.symbol_name, p2p.registration_date, p2p.registration_medium_type
#from catalog_entry as ce left outer join manufacturer as m on ce.manufacturer_id=m.manufacturer_id, catalog_entry as ce left outer join hw_product as hwp on ce.product_id=hwp.product_id, hw_product as hwp key join product_to_program as p2p left outer join application_program as ap on p2p.program_id=ap.program_id, hw_product as hwp left outer join symbol as s on hwp.symbol_id=s.symbol_id;

#create view subgroup_to_line as
#select SUBGROUP_ID, LINE_ID
#from subgroup_to_object
#join device_object
#on subgroup_to_object.device_object_id=device_object.device_object_id, device_object
#join device on device_object.device_id=device.device_id
#group by subgroup_id, line_id;

commit;

#
#  Datenbank-Version einfügen.
#
insert into ets_database_version values(3,24,1,'');

commit;

#
#  ID-Allocation einfügen.
#
insert into ID_ALLOCATION values(100);

commit;

#
#  Hersteller einfügen.
#
insert into manufacturer values(1,'Siemens',NULL);
insert into manufacturer values(2,'ABB',NULL);
insert into manufacturer values(3,'not assigned',NULL);
insert into manufacturer values(4,'Albrecht Jung',NULL);
insert into manufacturer values(5,'Bticino',NULL);
insert into manufacturer values(6,'Gebr. Berker',NULL);
insert into manufacturer values(7,'Busch-Jaeger Elektro',NULL);
insert into manufacturer values(8,'GIRA Giersiepen',NULL);
insert into manufacturer values(9,'Hager Electro',NULL);
insert into manufacturer values(10,'INSTA ELEKTRO',NULL);
insert into manufacturer values(11,'LEGRAND Appareillage électrique',NULL);
insert into manufacturer values(12,'Merten',NULL);
insert into manufacturer values(13,'MK Electric',NULL);
insert into manufacturer values(14,'ABB Elettrocondutture',NULL);
insert into manufacturer values(15,'Hanson Plc',NULL);
insert into manufacturer values(16,'Lindner',NULL);
insert into manufacturer values(17,'Heinrich Kopp',NULL);
insert into manufacturer values(18,'NIKO',NULL);
insert into manufacturer values(19,'PRESTO Gebr. Vedder',NULL);
insert into manufacturer values(20,'GEYER AG',NULL);
insert into manufacturer values(21,'CMC Carl Maier',NULL);
insert into manufacturer values(22,'Siedle & Söhne',NULL);
insert into manufacturer values(23,'A. Ahlstrom',NULL);
insert into manufacturer values(24,'Eberle',NULL);
insert into manufacturer values(25,'GEWISS',NULL);
insert into manufacturer values(26,'not assigned',NULL);
insert into manufacturer values(27,'Albert Ackermann',NULL);
insert into manufacturer values(28,'Schupa GmbH',NULL);
insert into manufacturer values(29,'Levy Fils',NULL);
insert into manufacturer values(30,'Feller',NULL);
insert into manufacturer values(31,'LK as',NULL);
insert into manufacturer values(32,'DEHN & SÖHNE',NULL);
insert into manufacturer values(33,'CRABTREE',NULL);
insert into manufacturer values(34,'Doepke',NULL);
insert into manufacturer values(36,'Paul Hochköpper',NULL);
insert into manufacturer values(37,'Altenburger Electronic',NULL);
insert into manufacturer values(38,'Striebel & John',NULL);
insert into manufacturer values(40,'Philips Lighting',NULL);
insert into manufacturer values(41,'Grässlin',NULL);
insert into manufacturer values(42,'Simon',NULL);
insert into manufacturer values(43,'not assigned',NULL);
insert into manufacturer values(44,'VIMAR',NULL);
insert into manufacturer values(45,'FELTEN & GUILLEAUME',NULL);
insert into manufacturer values(46,'Eltako',NULL);
insert into manufacturer values(48,'CERBERUS-GUINARD',NULL);
insert into manufacturer values(49,'Bosch-Siemens Haushaltsgeräte',NULL);
insert into manufacturer values(52,'RITTO-Werk Loh',NULL);
insert into manufacturer values(53,'Power Controls',NULL);
insert into manufacturer values(55,'ZUMTOBEL',NULL);
insert into manufacturer values(56,'not assigned',NULL);
insert into manufacturer values(57,'Phoenix Contact',NULL);
insert into manufacturer values(58,'OBO Bettermann',NULL);
insert into manufacturer values(60,'not assigned',NULL);
insert into manufacturer values(61,'WAGO Kontakttechnik',NULL);
insert into manufacturer values(62,'not assigned',NULL);
insert into manufacturer values(64,'Niessen',NULL);
insert into manufacturer values(65,'TEGUI Electronica',NULL);
insert into manufacturer values(66,'Wieland Electric',NULL);
insert into manufacturer values(67,'Hermann Kleinhuis',NULL);
insert into manufacturer values(68,'not assigned',NULL);
insert into manufacturer values(69,'Stiebel Eltron/Hydrotherm',NULL);
insert into manufacturer values(70,'Popp',NULL);
insert into manufacturer values(71,'Tehalit',NULL);
insert into manufacturer values(72,'Theben-Werk Zeitautomatik',NULL);
insert into manufacturer values(73,'Wilhelm Rutenbeck',NULL);
insert into manufacturer values(75,'Winkhaus',NULL);
insert into manufacturer values(76,'Robert Bosch',NULL);
insert into manufacturer values(77,'not assigned',NULL);
insert into manufacturer values(78,'Somfy',NULL);
insert into manufacturer values(79,'MENNEKES Elektrotechnik',NULL);
insert into manufacturer values(80,'Woertz',NULL);
insert into manufacturer values(81,'Viessmann Werke',NULL);
insert into manufacturer values(82,'Theodor HEIMEIER Metallwerk',NULL);
insert into manufacturer values(83,'Joh. Vaillant',NULL);
insert into manufacturer values(84,'DIEM Electronics',NULL);
insert into manufacturer values(85,'AMP Deutschland',NULL);
insert into manufacturer values(86,'Framatome Connectors Deutschland GmbH',NULL);
insert into manufacturer values(87,'DIEHL GmbH & Co. Controls Division',NULL);
insert into manufacturer values(88,'Grundig E.M.V.',NULL);
insert into manufacturer values(89,'Buderus Heiztechnik GmbH',NULL);
insert into manufacturer values(90,'SEF - ECOTEC',NULL);
insert into manufacturer values(91,'ELVOX Costruzzioni Elettriche SpA',NULL);
insert into manufacturer values(92,'DORMA GmbH + Co. KG',NULL);
insert into manufacturer values(93,'WindowMaster (VELUX)',NULL);
insert into manufacturer values(94,'Walther Werke',NULL);
insert into manufacturer values(95,'ORAS',NULL);
insert into manufacturer values(96,'Satchwell',NULL);
insert into manufacturer values(105,'RTS Automation',NULL);
insert into manufacturer values(213,'LichtVision',NULL);
insert into manufacturer values(214,'O.Y.L. Electronics',NULL);
insert into manufacturer values(215,'Galax System',NULL);
insert into manufacturer values(216,'Disch',NULL);
insert into manufacturer values(217,'Aucoteam',NULL);
insert into manufacturer values(218,'Luxmate Controls',NULL);
insert into manufacturer values(219,'Danfoss',NULL);
insert into manufacturer values(220,'AST GmbH',NULL);
insert into manufacturer values(221,'schaeper AUTOMATION GmbH',NULL);
insert into manufacturer values(222,'WILA Leuchten',NULL);
insert into manufacturer values(223,'Schlaps & Partner',NULL);
insert into manufacturer values(224,'IEL Systems',NULL);
insert into manufacturer values(225,'Lingg & Janke',NULL);
insert into manufacturer values(226,'Hassinger',NULL);
insert into manufacturer values(227,'Sauter',NULL);
insert into manufacturer values(228,'SIMU',NULL);
insert into manufacturer values(229,'Optimus S.A.',NULL);
insert into manufacturer values(230,'MTM Medientechnologie Harry Maier',NULL);
insert into manufacturer values(231,'Realtime Control Systems AG',NULL);
insert into manufacturer values(232,'HTS High Technology Systems AG',NULL);
insert into manufacturer values(233,'Amann GmbH',NULL);
insert into manufacturer values(234,'BERG Energiekontrollsysteme GmbH',NULL);
insert into manufacturer values(235,'Hüppe Form Sonnenschutzsysteme GmbH',NULL);
insert into manufacturer values(236,'INTERTEL S.r.l.',NULL);
insert into manufacturer values(237,'Oventrop KG',NULL);
insert into manufacturer values(238,'Multronic AG',NULL);
insert into manufacturer values(239,'IPAS GmbH',NULL);
insert into manufacturer values(240,'elero GmbH',NULL);
insert into manufacturer values(241,'Ardan Production and Industrial Controls Ltd.',NULL);
insert into manufacturer values(242,'not assigned',NULL);
insert into manufacturer values(243,'not assigned',NULL);
insert into manufacturer values(244,'ELKA Elektronik',NULL);
insert into manufacturer values(245,'ELEKTROANLAGEN D. NAGEL',NULL);
insert into manufacturer values(246,'Tridonic Bauelemente GmbH',NULL);
insert into manufacturer values(247,'Sika Systemtechnik GmbH',NULL);
insert into manufacturer values(248,'Stengler Gesellschaft',NULL);
insert into manufacturer values(249,'ELJO',NULL);
insert into manufacturer values(250,'EIBA s.c.',NULL);
insert into manufacturer values(252,'Hugo Müller',NULL);
insert into manufacturer values(253,'SBT-Landis&Staefa',NULL);
insert into manufacturer values(254,'APT',NULL);
insert into manufacturer values(255,'Techno Trend',NULL);

commit;

#
#  Hersteller-Kompatibilität einfügen.
#
insert into manufacturer_compatibility values(1,2,7);
insert into manufacturer_compatibility values(2,2,14);
insert into manufacturer_compatibility values(3,7,2);
insert into manufacturer_compatibility values(4,7,14);
insert into manufacturer_compatibility values(5,14,2);
insert into manufacturer_compatibility values(6,14,7);

commit;

#
#  Adress-Formate einfügen.
#
insert into address_format values(1,'English','Company','Department','POB','Street','Country',
'ZIP','City','Phone','Fax',NULL,NULL,NULL,NULL,NULL,NULL,1);
insert into address_format values(2,'Deutsch','Firma','Abteilung','Postfach','Strae','Land','PLZ',
'Stadt','Telefon','Fax',NULL,NULL,NULL,NULL,NULL,NULL,1);
insert into address_format values(3,'Franais','Societ','Dpartement','BP','Rue','Pays',
'Code postal','Ville','Tlphone','Fax',NULL,NULL,NULL,NULL,NULL,NULL,1);
insert into address_format values(20203,'Standard','Company','Department','POB','Street',
'Country','ZIP','City','Phone','Fax',NULL,NULL,NULL,NULL,NULL,NULL,1);

commit;

#
#  Adressen einfügen.
#
insert into address values(20204,20203,'EIBA',NULL,NULL,'Avenue de la Tanche 5',
'Belgium','B-1160','Bruxelles','0032/2/675-5020','0032/2/675-5028',NULL,NULL,NULL,NULL,NULL,NULL);

commit;

#
#  Assembler einfügen.
#
insert into ASSEMBLER values(1,'IASM',1);
insert into ASSEMBLER values(2,'IAR',2);
insert into ASSEMBLER values(3,'AD',3);

commit;

#
#  BCU-Typen einfügen.
#
insert into bcu_type values(1,'BCU 1','68HCO5B6');
insert into bcu_type values(2,'BIM M112','68HC11');
insert into bcu_type values(3,'BCU 2','68HCO5BE12');


#
#  Komponenten-Typen einfügen.
#
insert into component_type values (0,'Non EIB',1);
insert into component_type values (10,'Cable',1);
insert into component_type values (11,'Data Rail',1);
insert into component_type values (12,'Connector',1);
insert into component_type values (13,'Power Supply',32);
insert into component_type values (14,'Choke',64);
insert into component_type values (15,'Power + Choke',96);
insert into component_type values (16,'V.24 Conv.',30);
insert into component_type values (20,'EIB Device',30);
insert into component_type values (21,'BCU',30);
insert into component_type values (22,'Application Unit',30);
insert into component_type values (23,'Controller',30);
insert into component_type values (24,'Repeater',158);
insert into component_type values (25,'Line Coupler',158);
insert into component_type values (26,'Backbone Coupler',158);
insert into component_type values (27,'Coupler',158);

commit;


#
#  Währungen einfügen.
#
insert into CURRENCY values (1,'BEF');
insert into CURRENCY values (2,'DEM');
insert into CURRENCY values (3,'DKK');
insert into CURRENCY values (4,'ECU');
insert into CURRENCY values (5,'ESP');
insert into CURRENCY values (6,'FRF');
insert into CURRENCY values (7,'GBP');
insert into CURRENCY values (8,'GRD');
insert into CURRENCY values (9,'IEP');
insert into CURRENCY values (10,'ITL');
insert into CURRENCY values (11,'LUF');
insert into CURRENCY values (12,'NLG');
insert into CURRENCY values (13,'PTE');
insert into CURRENCY values (14,'USD');
insert into CURRENCY values (15,'SFR');

commit;

#
#  Geräte-Info-Werte einfügen.
#
insert into device_info_value values(20673,20394,5507,NULL,NULL,NULL);
insert into device_info_value values(20674,20446,5515,NULL,NULL,NULL);
insert into device_info_value values(20675,20447,5518,0,21,'is too low.');
insert into device_info_value values(20676,20447,5518,22,28,'is normal.');
insert into device_info_value values(20677,20447,5518,29,1000,'is too high.');
insert into device_info_value values(20678,20448,5519,NULL,NULL,NULL);
insert into device_info_value values(20679,20572,5521,NULL,NULL,NULL);
insert into device_info_value values(20680,20573,5522,NULL,NULL,NULL);
insert into device_info_value values(20681,20574,5520,NULL,NULL,NULL);
insert into device_info_value values(20682,20449,5533,NULL,NULL,NULL);
insert into device_info_value values(20683,20575,5516,0,0,'error detected.');
insert into device_info_value values(20684,20575,5517,1,1,'is ok.');
insert into device_info_value values(20685,20576,5516,0,0,'error detected.');
insert into device_info_value values(20686,20576,5517,1,1,'is ok.');
insert into device_info_value values(20687,20577,5516,0,0,'error detected.');
insert into device_info_value values(20688,20577,5517,1,1,'is ok.');
insert into device_info_value values(20689,20578,5516,0,0,'error detected.');
insert into device_info_value values(20690,20578,5517,1,1,'is ok.');
insert into device_info_value values(20691,20579,5516,0,0,'overflow detected.');
insert into device_info_value values(20692,20579,5517,1,1,'is ok.');
insert into device_info_value values(20693,20580,5516,0,0,'error detected.');
insert into device_info_value values(20694,20580,5517,1,1,'is ok.');
insert into device_info_value values(20695,20581,5516,0,0,'error detected.');
insert into device_info_value values(20696,20581,5517,1,1,'is ok.');
insert into device_info_value values(20697,20450,5524,NULL,NULL,NULL);
insert into device_info_value values(20698,20582,5525,0,0,'is disabled.');
insert into device_info_value values(20699,20582,5525,1,1,'is enabled.');
insert into device_info_value values(20700,20583,5526,0,0,'is disabled.');
insert into device_info_value values(20701,20583,5526,1,1,'is enabled.');
insert into device_info_value values(20702,20584,5527,0,0,'is disabled.');
insert into device_info_value values(20703,20584,5527,1,1,'is enabled.');
insert into device_info_value values(20704,20585,5528,0,0,'is disabled.');
insert into device_info_value values(20705,20585,5528,1,1,'is enabled.');
insert into device_info_value values(20706,20451,5523,NULL,NULL,NULL);
insert into device_info_value values(20707,20395,5507,NULL,NULL,NULL);
insert into device_info_value values(20708,20452,5515,NULL,NULL,NULL);
insert into device_info_value values(20709,20453,5518,0,21,'is too low.');
insert into device_info_value values(20710,20453,5518,22,28,'is normal.');
insert into device_info_value values(20711,20453,5518,29,1000,'is too high.');
insert into device_info_value values(20712,20454,5519,NULL,NULL,NULL);
insert into device_info_value values(20713,20586,5521,NULL,NULL,NULL);
insert into device_info_value values(20714,20587,5522,NULL,NULL,NULL);
insert into device_info_value values(20715,20588,5520,NULL,NULL,NULL);
insert into device_info_value values(20716,20455,5533,NULL,NULL,NULL);
insert into device_info_value values(20717,20589,5516,0,0,'error detected.');
insert into device_info_value values(20718,20589,5517,1,1,'is ok.');
insert into device_info_value values(20719,20590,5516,0,0,'error detected.');
insert into device_info_value values(20720,20590,5517,1,1,'is ok.');
insert into device_info_value values(20721,20591,5516,0,0,'error detected.');
insert into device_info_value values(20722,20591,5517,1,1,'is ok.');
insert into device_info_value values(20723,20592,5516,0,0,'error detected.');
insert into device_info_value values(20724,20592,5517,1,1,'is ok.');
insert into device_info_value values(20725,20593,5516,0,0,'overflow detected.');
insert into device_info_value values(20726,20593,5517,1,1,'is ok.');
insert into device_info_value values(20727,20594,5516,0,0,'error detected.');
insert into device_info_value values(20728,20594,5517,1,1,'is ok.');
insert into device_info_value values(20729,20595,5516,0,0,'error detected.');
insert into device_info_value values(20730,20595,5517,1,1,'is ok.');
insert into device_info_value values(20731,20456,5524,NULL,NULL,NULL);
insert into device_info_value values(20732,20596,5525,0,0,'is disabled.');
insert into device_info_value values(20733,20596,5525,1,1,'is enabled.');
insert into device_info_value values(20734,20597,5526,0,0,'is disabled.');
insert into device_info_value values(20735,20597,5526,1,1,'is enabled.');
insert into device_info_value values(20736,20598,5527,0,0,'is disabled.');
insert into device_info_value values(20737,20598,5527,1,1,'is enabled.');
insert into device_info_value values(20738,20599,5528,0,0,'is disabled.');
insert into device_info_value values(20739,20599,5528,1,1,'is enabled.');
insert into device_info_value values(20740,20457,5523,NULL,NULL,NULL);
insert into device_info_value values(20741,20396,5507,NULL,NULL,NULL);
insert into device_info_value values(20742,20458,5515,NULL,NULL,NULL);
insert into device_info_value values(20743,20459,5518,0,21,'is too low.');
insert into device_info_value values(20744,20459,5518,22,28,'is normal.');
insert into device_info_value values(20745,20459,5518,29,1000,'is too high.');
insert into device_info_value values(20746,20460,5519,NULL,NULL,NULL);
insert into device_info_value values(20747,20600,5521,NULL,NULL,NULL);
insert into device_info_value values(20748,20601,5522,NULL,NULL,NULL);
insert into device_info_value values(20749,20461,5520,NULL,NULL,NULL);
insert into device_info_value values(20750,20397,5533,NULL,NULL,NULL);
insert into device_info_value values(20751,20462,5516,0,0,'error detected.');
insert into device_info_value values(20752,20462,5517,1,1,'is ok.');
insert into device_info_value values(20753,20463,5516,0,0,'error detected.');
insert into device_info_value values(20754,20463,5517,1,1,'is ok.');
insert into device_info_value values(20755,20464,5516,0,0,'error detected.');
insert into device_info_value values(20756,20464,5517,1,1,'is ok.');
insert into device_info_value values(20757,20465,5516,0,0,'error detected.');
insert into device_info_value values(20758,20465,5517,1,1,'is ok.');
insert into device_info_value values(20759,20466,5516,0,0,'overflow detected.');
insert into device_info_value values(20760,20466,5517,1,1,'is ok.');
insert into device_info_value values(20761,20467,5516,0,0,'error detected.');
insert into device_info_value values(20762,20467,5517,1,1,'is ok.');
insert into device_info_value values(20763,20468,5516,0,0,'error detected.');
insert into device_info_value values(20764,20468,5517,1,1,'is ok.');
insert into device_info_value values(20765,20398,5524,NULL,NULL,NULL);
insert into device_info_value values(20766,20469,5525,0,0,'is disabled.');
insert into device_info_value values(20767,20469,5525,1,1,'is enabled.');
insert into device_info_value values(20768,20470,5526,0,0,'is disabled.');
insert into device_info_value values(20769,20470,5526,1,1,'is enabled.');
insert into device_info_value values(20770,20471,5527,0,0,'is disabled.');
insert into device_info_value values(20771,20471,5527,1,1,'is enabled.');
insert into device_info_value values(20772,20472,5528,0,0,'is disabled.');
insert into device_info_value values(20773,20472,5528,1,1,'is enabled.');
insert into device_info_value values(20774,20399,5523,NULL,NULL,NULL);
insert into device_info_value values(20775,20386,5507,NULL,NULL,NULL);
insert into device_info_value values(20776,20400,5515,NULL,NULL,NULL);
insert into device_info_value values(20777,20401,5519,NULL,NULL,NULL);
insert into device_info_value values(20778,20473,5521,NULL,NULL,NULL);
insert into device_info_value values(20779,20474,5522,NULL,NULL,NULL);
insert into device_info_value values(20780,20475,5520,NULL,NULL,NULL);
insert into device_info_value values(20781,20402,5533,NULL,NULL,NULL);
insert into device_info_value values(20782,20476,5516,0,0,'error detected.');
insert into device_info_value values(20783,20476,5517,1,1,'is ok.');
insert into device_info_value values(20784,20477,5516,0,0,'error detected.');
insert into device_info_value values(20785,20477,5517,1,1,'is ok.');
insert into device_info_value values(20786,20478,5516,0,0,'error detected.');
insert into device_info_value values(20787,20478,5517,1,1,'is ok.');
insert into device_info_value values(20788,20479,5516,0,0,'error detected.');
insert into device_info_value values(20789,20479,5517,1,1,'is ok.');
insert into device_info_value values(20790,20480,5516,0,0,'overflow detected.');
insert into device_info_value values(20791,20480,5517,1,1,'is ok.');
insert into device_info_value values(20792,20481,5516,0,0,'error detected.');
insert into device_info_value values(20793,20481,5517,1,1,'is ok.');
insert into device_info_value values(20794,20482,5516,0,0,'error detected.');
insert into device_info_value values(20795,20482,5517,1,1,'is ok.');
insert into device_info_value values(20796,20403,5524,NULL,NULL,NULL);
insert into device_info_value values(20797,20483,5525,0,0,'is disabled.');
insert into device_info_value values(20798,20483,5525,1,1,'is enabled.');
insert into device_info_value values(20799,20484,5526,0,0,'is disabled.');
insert into device_info_value values(20800,20484,5526,1,1,'is enabled.');
insert into device_info_value values(20801,20485,5527,0,0,'is disabled.');
insert into device_info_value values(20802,20485,5527,1,1,'is enabled.');
insert into device_info_value values(20803,20486,5528,0,0,'is disabled.');
insert into device_info_value values(20804,20486,5528,1,1,'is enabled.');
insert into device_info_value values(20805,20404,5523,NULL,NULL,NULL);
insert into device_info_value values(20806,20405,5555,NULL,NULL,NULL);
insert into device_info_value values(20807,20406,5552,NULL,NULL,NULL);
insert into device_info_value values(20808,20487,5551,1,1,'is normal');
insert into device_info_value values(20809,20488,5554,1,1,'is deactivated.');
insert into device_info_value values(20810,20407,5515,NULL,NULL,NULL);
insert into device_info_value values(20811,20408,5519,NULL,NULL,NULL);
insert into device_info_value values(20812,20490,5521,NULL,NULL,NULL);
insert into device_info_value values(20813,20491,5522,NULL,NULL,NULL);
insert into device_info_value values(20814,20492,5520,NULL,NULL,NULL);
insert into device_info_value values(20815,20409,5533,NULL,NULL,NULL);
insert into device_info_value values(20816,20493,5516,0,0,'error detected.');
insert into device_info_value values(20817,20493,5517,1,1,'is ok.');
insert into device_info_value values(20818,20494,5516,0,0,'error detected.');
insert into device_info_value values(20819,20494,5517,1,1,'is ok.');
insert into device_info_value values(20820,20495,5516,0,0,'error detected.');
insert into device_info_value values(20821,20495,5517,1,1,'is ok.');
insert into device_info_value values(20822,20496,5516,0,0,'error detected.');
insert into device_info_value values(20823,20496,5517,1,1,'is ok.');
insert into device_info_value values(20824,20497,5516,0,0,'overflow detected.');
insert into device_info_value values(20825,20497,5517,1,1,'is ok.');
insert into device_info_value values(20826,20498,5516,0,0,'error detected.');
insert into device_info_value values(20827,20498,5517,1,1,'is ok.');
insert into device_info_value values(20828,20499,5516,0,0,'error detected.');
insert into device_info_value values(20829,20499,5517,1,1,'is ok.');
insert into device_info_value values(20830,20410,5524,NULL,NULL,NULL);
insert into device_info_value values(20831,20500,5525,0,0,'is disabled.');
insert into device_info_value values(20832,20500,5525,1,1,'is enabled.');
insert into device_info_value values(20833,20501,5526,0,0,'is disabled.');
insert into device_info_value values(20834,20501,5526,1,1,'is enabled.');
insert into device_info_value values(20835,20502,5527,0,0,'is disabled.');
insert into device_info_value values(20836,20502,5527,1,1,'is enabled.');
insert into device_info_value values(20837,20503,5528,0,0,'is disabled.');
insert into device_info_value values(20838,20503,5528,1,1,'is enabled.');
insert into device_info_value values(20839,20411,5523,NULL,NULL,NULL);
insert into device_info_value values(20840,20412,5555,NULL,NULL,NULL);
insert into device_info_value values(20841,20413,5552,NULL,NULL,NULL);
insert into device_info_value values(20842,20504,5551,1,1,'is normal');
insert into device_info_value values(20843,20505,5554,1,1,'is deactivated.');
insert into device_info_value values(20844,20505,5554,0,0,'is activated.');
insert into device_info_value values(20845,20506,5553,3,3,'1200');
insert into device_info_value values(20846,20388,5507,NULL,NULL,NULL);
insert into device_info_value values(20847,20414,5515,NULL,NULL,NULL);
insert into device_info_value values(20848,20415,5534,0,0,'is NOT set.');
insert into device_info_value values(20849,20415,5534,1,1,'is set.');
insert into device_info_value values(20850,20416,5519,NULL,NULL,NULL);
insert into device_info_value values(20851,20507,5521,NULL,NULL,NULL);
insert into device_info_value values(20852,20508,5522,NULL,NULL,NULL);
insert into device_info_value values(20853,20509,5520,NULL,NULL,NULL);
insert into device_info_value values(20854,20417,5535,NULL,NULL,NULL);
insert into device_info_value values(20855,20510,5536,0,0,'is unloaded.');
insert into device_info_value values(20856,20510,5536,1,1,'is loaded.');
insert into device_info_value values(20857,20510,5536,2,2,'is loading.');
insert into device_info_value values(20858,20510,5536,3,3,'error detected.');
insert into device_info_value values(20859,20510,5536,4,4,'is unloading.');
insert into device_info_value values(20860,20511,5537,0,0,'is unloaded.');
insert into device_info_value values(20861,20511,5537,1,1,'is loaded.');
insert into device_info_value values(20862,20511,5537,2,2,'is loading.');
insert into device_info_value values(20863,20511,5537,3,3,'error detected.');
insert into device_info_value values(20864,20511,5537,4,4,'is unloading.');
insert into device_info_value values(20865,20512,5538,0,0,'is unloaded.');
insert into device_info_value values(20866,20512,5538,1,1,'is loaded.');
insert into device_info_value values(20867,20512,5538,2,2,'is loading.');
insert into device_info_value values(20868,20512,5538,3,3,'error detected.');
insert into device_info_value values(20869,20512,5538,4,4,'is unloading.');
insert into device_info_value values(20870,20513,5539,0,0,'is unloaded.');
insert into device_info_value values(20871,20513,5539,1,1,'is loaded.');
insert into device_info_value values(20872,20513,5539,2,2,'is loading.');
insert into device_info_value values(20873,20513,5539,3,3,'error detected.');
insert into device_info_value values(20874,20513,5539,4,4,'is unloading.');
insert into device_info_value values(20875,20418,5545,NULL,NULL,NULL);
insert into device_info_value values(20876,20514,5547,0,0,'is halted.');
insert into device_info_value values(20877,20514,5547,1,1,'is running.');
insert into device_info_value values(20878,20514,5547,2,2,'is ready.');
insert into device_info_value values(20879,20514,5547,3,3,'is terminated.');
insert into device_info_value values(20880,20515,5548,0,0,'is halted.');
insert into device_info_value values(20881,20515,5548,1,1,'is running.');
insert into device_info_value values(20882,20515,5548,2,2,'is ready.');
insert into device_info_value values(20883,20515,5548,3,3,'is terminated.');
insert into device_info_value values(20884,20389,5507,NULL,NULL,NULL);
insert into device_info_value values(20885,20419,5515,NULL,NULL,NULL);
insert into device_info_value values(20886,20420,5519,NULL,NULL,NULL);
insert into device_info_value values(20887,20516,5521,NULL,NULL,NULL);
insert into device_info_value values(20888,20517,5522,NULL,NULL,NULL);
insert into device_info_value values(20889,20518,5520,NULL,NULL,NULL);
insert into device_info_value values(20890,20421,5533,NULL,NULL,NULL);
insert into device_info_value values(20891,20519,5516,0,0,'error detected.');
insert into device_info_value values(20892,20519,5517,1,1,'is ok.');
insert into device_info_value values(20893,20520,5516,0,0,'error detected.');
insert into device_info_value values(20894,20520,5517,1,1,'is ok.');
insert into device_info_value values(20895,20521,5516,0,0,'error detected.');
insert into device_info_value values(20896,20521,5517,1,1,'is ok.');
insert into device_info_value values(20897,20522,5516,0,0,'error detected.');
insert into device_info_value values(20898,20522,5517,1,1,'is ok.');
insert into device_info_value values(20899,20523,5516,0,0,'overflow detected.');
insert into device_info_value values(20900,20523,5517,1,1,'is ok.');
insert into device_info_value values(20901,20524,5516,0,0,'error detected.');
insert into device_info_value values(20902,20524,5517,1,1,'is ok.');
insert into device_info_value values(20903,20525,5516,0,0,'error detected.');
insert into device_info_value values(20904,20525,5517,1,1,'is ok.');
insert into device_info_value values(20905,20422,5524,NULL,NULL,NULL);
insert into device_info_value values(20906,20526,5525,0,0,'is disabled.');
insert into device_info_value values(20907,20526,5525,1,1,'is enabled.');
insert into device_info_value values(20908,20527,5526,0,0,'is disabled.');
insert into device_info_value values(20909,20527,5526,1,1,'is enabled.');
insert into device_info_value values(20910,20528,5527,0,0,'is disabled.');
insert into device_info_value values(20911,20528,5527,1,1,'is enabled.');
insert into device_info_value values(20912,20529,5528,0,0,'is disabled.');
insert into device_info_value values(20913,20529,5528,1,1,'is enabled.');
insert into device_info_value values(20914,20423,5523,NULL,NULL,NULL);
insert into device_info_value values(20915,20390,5507,NULL,NULL,NULL);
insert into device_info_value values(20916,20424,5515,NULL,NULL,NULL);
insert into device_info_value values(20917,20425,5523,NULL,NULL,NULL);
insert into device_info_value values(20918,20426,5549,NULL,NULL,NULL);
insert into device_info_value values(20919,20427,5550,NULL,NULL,NULL);
insert into device_info_value values(20920,20530,5543,NULL,NULL,NULL);
insert into device_info_value values(20921,20531,5543,NULL,NULL,NULL);
insert into device_info_value values(20922,20532,5544,NULL,NULL,NULL);
insert into device_info_value values(20923,20533,5544,NULL,NULL,NULL);
insert into device_info_value values(20924,20534,5533,NULL,NULL,NULL);
insert into device_info_value values(20925,20535,5533,NULL,NULL,NULL);
insert into device_info_value values(20926,20536,5533,NULL,NULL,NULL);
insert into device_info_value values(20927,20602,5516,0,0,'error detected.');
insert into device_info_value values(20928,20602,5517,1,1,'is ok.');
insert into device_info_value values(20929,20603,5516,0,0,'error detected.');
insert into device_info_value values(20930,20603,5517,1,1,'is ok.');
insert into device_info_value values(20931,20604,5516,0,0,'error detected.');
insert into device_info_value values(20932,20604,5517,1,1,'is ok.');
insert into device_info_value values(20933,20605,5516,0,0,'error detected.');
insert into device_info_value values(20934,20605,5517,1,1,'is ok.');
insert into device_info_value values(20935,20606,5516,0,0,'overflow detected.');
insert into device_info_value values(20936,20606,5517,1,1,'is ok.');
insert into device_info_value values(20937,20607,5516,0,0,'error detected.');
insert into device_info_value values(20938,20607,5517,1,1,'is ok.');
insert into device_info_value values(20939,20608,5516,0,0,'error detected.');
insert into device_info_value values(20940,20608,5517,1,1,'is ok.');
insert into device_info_value values(20941,20537,5533,NULL,NULL,NULL);
insert into device_info_value values(20942,20609,5516,0,0,'error detected.');
insert into device_info_value values(20943,20609,5517,1,1,'is ok.');
insert into device_info_value values(20944,20610,5516,0,0,'error detected.');
insert into device_info_value values(20945,20610,5517,1,1,'is ok.');
insert into device_info_value values(20946,20611,5516,0,0,'error detected.');
insert into device_info_value values(20947,20611,5517,1,1,'is ok.');
insert into device_info_value values(20948,20612,5516,0,0,'error detected.');
insert into device_info_value values(20949,20612,5517,1,1,'is ok.');
insert into device_info_value values(20950,20613,5516,0,0,'overflow detected.');
insert into device_info_value values(20951,20613,5517,1,1,'is ok.');
insert into device_info_value values(20952,20614,5516,0,0,'error detected.');
insert into device_info_value values(20953,20614,5517,1,1,'is ok.');
insert into device_info_value values(20954,20615,5516,0,0,'error detected.');
insert into device_info_value values(20955,20615,5517,1,1,'is ok.');
insert into device_info_value values(20956,20428,5542,13,13,'is relay coupler.');
insert into device_info_value values(20957,20428,5542,14,14,'is test mode.');
insert into device_info_value values(20958,20428,5542,11,11,'is line coupler.');
insert into device_info_value values(20959,20428,5542,7,7,'is bus coupler.');
insert into device_info_value values(20960,20616,5543,1,1,'will be tested.');
insert into device_info_value values(20961,20616,5543,2,2,'error, pass through all telegrams.');
insert into device_info_value values(20962,20616,5543,3,3,', ignore errors.');
insert into device_info_value values(20963,20617,5543,0,0,'main groups 14/15 will NOT be transferred.');
insert into device_info_value values(20964,20617,5543,1,1,'main groups 14/15 will be transferred.');
insert into device_info_value values(20965,20616,5543,0,0,'will NOT be tested.');
insert into device_info_value values(20966,20618,5543,0,0,'Filtering of physical address disabled.');
insert into device_info_value values(20967,20618,5543,1,1,'Filtering of physical address enabled.');
insert into device_info_value values(20968,20619,5543,0,0,'Filtering of physical address disabled.');
insert into device_info_value values(20969,20619,5543,1,1,'Filtering of physical address enabled.');
insert into device_info_value values(20970,20620,5543,0,0,'IACK on not matching addressing enabled.');
insert into device_info_value values(20971,20620,5543,1,1,'IACK on not matching addressing disabled.');
insert into device_info_value values(20972,20621,5543,0,0,'IACK on not matching addressing enabled.');
insert into device_info_value values(20973,20621,5543,1,1,'IACK on not matching addressing disabled.');
insert into device_info_value values(20974,20622,5517,1,1,'is ok.');
insert into device_info_value values(20975,20622,5543,2,2,'error, pass through all telegrams.');
insert into device_info_value values(20976,20622,5543,3,3,', ignore errors.');
insert into device_info_value values(20977,20623,5543,0,0,'main groups 14/15 will NOT be transferred.');
insert into device_info_value values(20978,20623,5543,1,1,'main groups 14/15 will be transferred.');
insert into device_info_value values(20979,20622,5516,0,0,'error detected.');
insert into device_info_value values(20980,20624,5543,0,0,'Filtering of physical address disabled.');
insert into device_info_value values(20981,20624,5543,1,1,'Filtering of physical address enabled.');
insert into device_info_value values(20982,20625,5543,0,0,'Filtering of physical address disabled.');
insert into device_info_value values(20983,20625,5543,1,1,'Filtering of physical address enabled.');
insert into device_info_value values(20984,20626,5543,0,0,'IACK on not matching addressing enabled.');
insert into device_info_value values(20985,20626,5543,1,1,'IACK on not matching addressing disabled.');
insert into device_info_value values(20986,20627,5543,0,0,'IACK on not matching addressing enabled.');
insert into device_info_value values(20987,20627,5543,1,1,'IACK on not matching addressing disabled.');
insert into device_info_value values(20988,20628,5544,1,1,'Free group telegram handling.');
insert into device_info_value values(20989,20628,5544,2,2,'Group telegrams are blocked.');
insert into device_info_value values(20990,20628,5544,3,3,'Normal group telegram handling.');
insert into device_info_value values(20991,20629,5544,1,1,'Free group telegram handling.');
insert into device_info_value values(20992,20629,5544,2,2,'Group telegrams are blocked.');
insert into device_info_value values(20993,20629,5544,3,3,'Normal group telegram handling.');
insert into device_info_value values(20994,20630,5544,1,1,'Free group telegram handling.');
insert into device_info_value values(20995,20630,5544,2,2,'Group telegrams are blocked.');
insert into device_info_value values(20996,20630,5544,3,3,'Normal group telegram handling.');
insert into device_info_value values(20997,20631,5544,1,1,'Free group telegram handling.');
insert into device_info_value values(20998,20631,5544,2,2,'Group telegrams are blocked.');
insert into device_info_value values(20999,20631,5544,3,3,'Normal group telegram handling.');
insert into device_info_value values(21000,20662,5516,0,0,'error detected.');
insert into device_info_value values(21001,20662,5517,1,1,'is ok.');
insert into device_info_value values(21002,20663,5516,0,0,'error after reset detected.');
insert into device_info_value values(21003,20663,5517,1,1,'is ok.');
insert into device_info_value values(21004,20664,5516,0,0,'is not permissible.');
insert into device_info_value values(21005,20664,5517,1,1,'is ok.');
insert into device_info_value values(21006,20665,5516,0,0,'error detected.');
insert into device_info_value values(21007,20665,5517,1,1,'is ok.');
insert into device_info_value values(21008,20666,5516,0,0,'error after reset detected.');
insert into device_info_value values(21009,20666,5517,1,1,'is ok.');
insert into device_info_value values(21010,20667,5516,0,0,'is not permissible.');
insert into device_info_value values(21011,20667,5517,1,1,'is ok.');
insert into device_info_value values(21012,20391,5507,NULL,NULL,NULL);
insert into device_info_value values(21013,20429,5515,NULL,NULL,NULL);
insert into device_info_value values(21014,20430,5523,NULL,NULL,NULL);
insert into device_info_value values(21015,20432,5550,NULL,NULL,NULL);
insert into device_info_value values(21016,20538,5543,NULL,NULL,NULL);
insert into device_info_value values(21017,20539,5543,NULL,NULL,NULL);
insert into device_info_value values(21018,20540,5544,NULL,NULL,NULL);
insert into device_info_value values(21019,20541,5544,NULL,NULL,NULL);
insert into device_info_value values(21020,20542,5533,NULL,NULL,NULL);
insert into device_info_value values(21021,20543,5533,NULL,NULL,NULL);
insert into device_info_value values(21022,20544,5533,NULL,NULL,NULL);
insert into device_info_value values(21023,20632,5516,0,0,'error detected.');
insert into device_info_value values(21024,20632,5517,1,1,'is ok.');
insert into device_info_value values(21025,20633,5516,0,0,'error detected.');
insert into device_info_value values(21026,20633,5517,1,1,'is ok.');
insert into device_info_value values(21027,20634,5516,0,0,'error detected.');
insert into device_info_value values(21028,20634,5517,1,1,'is ok.');
insert into device_info_value values(21029,20635,5516,0,0,'error detected.');
insert into device_info_value values(21030,20635,5517,1,1,'is ok.');
insert into device_info_value values(21031,20636,5516,0,0,'overflow detected.');
insert into device_info_value values(21032,20636,5517,1,1,'is ok.');
insert into device_info_value values(21033,20637,5516,0,0,'error detected.');
insert into device_info_value values(21034,20637,5517,1,1,'is ok.');
insert into device_info_value values(21035,20638,5516,0,0,'error detected.');
insert into device_info_value values(21036,20638,5517,1,1,'is ok.');
insert into device_info_value values(21037,20545,5533,NULL,NULL,NULL);
insert into device_info_value values(21038,20639,5516,0,0,'error detected.');
insert into device_info_value values(21039,20639,5517,1,1,'is ok.');
insert into device_info_value values(21040,20640,5516,0,0,'error detected.');
insert into device_info_value values(21041,20640,5517,1,1,'is ok.');
insert into device_info_value values(21042,20641,5516,0,0,'error detected.');
insert into device_info_value values(21043,20641,5517,1,1,'is ok.');
insert into device_info_value values(21044,20642,5516,0,0,'error detected.');
insert into device_info_value values(21045,20642,5517,1,1,'is ok.');
insert into device_info_value values(21046,20643,5516,0,0,'overflow detected.');
insert into device_info_value values(21047,20643,5517,1,1,'is ok.');
insert into device_info_value values(21048,20644,5516,0,0,'error detected.');
insert into device_info_value values(21049,20644,5517,1,1,'is ok.');
insert into device_info_value values(21050,20645,5516,0,0,'error detected.');
insert into device_info_value values(21051,20645,5517,1,1,'is ok.');
insert into device_info_value values(21052,20433,5542,13,13,'is relay coupler.');
insert into device_info_value values(21053,20433,5542,14,14,'is test mode.');
insert into device_info_value values(21054,20433,5542,11,11,'is line coupler.');
insert into device_info_value values(21055,20433,5542,7,7,'is bus coupler.');
insert into device_info_value values(21056,20646,5543,1,1,'will be tested.');
insert into device_info_value values(21057,20646,5543,2,2,'error, pass through all telegrams.');
insert into device_info_value values(21058,20646,5543,3,3,', ignore errors.');
insert into device_info_value values(21059,20647,5543,0,0,'main groups 14/15 will NOT be transferred.');
insert into device_info_value values(21060,20647,5543,1,1,'main groups 14/15 will be transferred.');
insert into device_info_value values(21061,20646,5543,0,0,'will NOT be tested.');
insert into device_info_value values(21062,20648,5543,0,0,'Filtering of physical address disabled.');
insert into device_info_value values(21063,20648,5543,1,1,'Filtering of physical address enabled.');
insert into device_info_value values(21064,20649,5543,0,0,'Filtering of physical address disabled.');
insert into device_info_value values(21065,20649,5543,1,1,'Filtering of physical address enabled.');
insert into device_info_value values(21066,20650,5543,0,0,'IACK on not matching addressing enabled.');
insert into device_info_value values(21067,20650,5543,1,1,'IACK on not matching addressing disabled.');
insert into device_info_value values(21068,20651,5543,0,0,'IACK on not matching addressing enabled.');
insert into device_info_value values(21069,20651,5543,1,1,'IACK on not matching addressing disabled.');
insert into device_info_value values(21070,20652,5517,1,1,'is ok.');
insert into device_info_value values(21071,20652,5543,2,2,'error, pass through all telegrams.');
insert into device_info_value values(21072,20652,5543,3,3,', ignore errors.');
insert into device_info_value values(21073,20653,5543,0,0,'main groups 14/15 will NOT be transferred.');
insert into device_info_value values(21074,20653,5543,1,1,'main groups 14/15 will be transferred.');
insert into device_info_value values(21075,20652,5516,0,0,'error detected.');
insert into device_info_value values(21076,20654,5543,0,0,'Filtering of physical address disabled.');
insert into device_info_value values(21077,20654,5543,1,1,'Filtering of physical address enabled.');
insert into device_info_value values(21078,20655,5543,0,0,'Filtering of physical address disabled.');
insert into device_info_value values(21079,20655,5543,1,1,'Filtering of physical address enabled.');
insert into device_info_value values(21080,20656,5543,0,0,'IACK on not matching addressing enabled.');
insert into device_info_value values(21081,20656,5543,1,1,'IACK on not matching addressing disabled.');
insert into device_info_value values(21082,20657,5543,0,0,'IACK on not matching addressing enabled.');
insert into device_info_value values(21083,20657,5543,1,1,'IACK on not matching addressing disabled.');
insert into device_info_value values(21084,20658,5544,1,1,'Free group telegram handling.');
insert into device_info_value values(21085,20658,5544,2,2,'Group telegrams are blocked.');
insert into device_info_value values(21086,20658,5544,3,3,'Normal group telegram handling.');
insert into device_info_value values(21087,20659,5544,1,1,'Free group telegram handling.');
insert into device_info_value values(21088,20659,5544,2,2,'Group telegrams are blocked.');
insert into device_info_value values(21089,20659,5544,3,3,'Normal group telegram handling.');
insert into device_info_value values(21090,20660,5544,1,1,'Free group telegram handling.');
insert into device_info_value values(21091,20660,5544,2,2,'Group telegrams are blocked.');
insert into device_info_value values(21092,20660,5544,3,3,'Normal group telegram handling.');
insert into device_info_value values(21093,20661,5544,1,1,'Free group telegram handling.');
insert into device_info_value values(21094,20661,5544,2,2,'Group telegrams are blocked.');
insert into device_info_value values(21095,20661,5544,3,3,'Normal group telegram handling.');
insert into device_info_value values(21096,20668,5516,0,0,'error detected.');
insert into device_info_value values(21097,20668,5517,1,1,'is ok.');
insert into device_info_value values(21098,20669,5516,0,0,'error after reset detected.');
insert into device_info_value values(21099,20669,5517,1,1,'is ok.');
insert into device_info_value values(21100,20670,5516,0,0,'is not permissible.');
insert into device_info_value values(21101,20670,5517,1,1,'is ok.');
insert into device_info_value values(21102,20671,5516,0,0,'error detected.');
insert into device_info_value values(21103,20671,5517,1,1,'is ok.');
insert into device_info_value values(21104,20672,5516,0,0,'error after reset detected.');
insert into device_info_value values(21105,20672,5517,1,1,'is ok.');
insert into device_info_value values(21106,20488,5554,0,0,'is activated.');
insert into device_info_value values(21107,20489,5553,3,3,'1200');
insert into device_info_value values(21108,20387,5507,NULL,NULL,NULL);
insert into device_info_value values(21109,20431,5549,NULL,NULL,NULL);
insert into device_info_value values(21110,20487,5551,0,0,'is reduced.');
insert into device_info_value values(21111,20504,5551,0,0,'is reduced.');
insert into device_info_value values(21112,20392,5507,NULL,NULL,NULL);
insert into device_info_value values(21113,20434,5515,NULL,NULL,NULL);
insert into device_info_value values(21114,20435,5534,0,0,'is NOT set.');
insert into device_info_value values(21115,20435,5534,1,1,'is set.');
insert into device_info_value values(21116,20436,5519,NULL,NULL,NULL);
insert into device_info_value values(21117,20546,5521,NULL,NULL,NULL);
insert into device_info_value values(21118,20547,5522,NULL,NULL,NULL);
insert into device_info_value values(21119,20548,5520,NULL,NULL,NULL);
insert into device_info_value values(21120,20437,5535,NULL,NULL,NULL);
insert into device_info_value values(21121,20549,5536,0,0,'is unloaded.');
insert into device_info_value values(21122,20549,5536,1,1,'is loaded.');
insert into device_info_value values(21123,20549,5536,2,2,'is loading.');
insert into device_info_value values(21124,20549,5536,3,3,'error detected.');
insert into device_info_value values(21125,20549,5536,4,4,'is unloading.');
insert into device_info_value values(21126,20550,5537,0,0,'is unloaded.');
insert into device_info_value values(21127,20550,5537,1,1,'is loaded.');
insert into device_info_value values(21128,20550,5537,2,2,'is loading.');
insert into device_info_value values(21129,20550,5537,3,3,'error detected.');
insert into device_info_value values(21130,20550,5537,4,4,'is unloading.');
insert into device_info_value values(21131,20551,5538,0,0,'is unloaded.');
insert into device_info_value values(21132,20551,5538,1,1,'is loaded.');
insert into device_info_value values(21133,20551,5538,2,2,'is loading.');
insert into device_info_value values(21134,20551,5538,3,3,'error detected.');
insert into device_info_value values(21135,20551,5538,4,4,'is unloading.');
insert into device_info_value values(21136,20552,5539,0,0,'is unloaded.');
insert into device_info_value values(21137,20552,5539,1,1,'is loaded.');
insert into device_info_value values(21138,20552,5539,2,2,'is loading.');
insert into device_info_value values(21139,20552,5539,3,3,'error detected.');
insert into device_info_value values(21140,20552,5539,4,4,'is unloading.');
insert into device_info_value values(21141,20438,5545,NULL,NULL,NULL);
insert into device_info_value values(21142,20553,5547,0,0,'is halted.');
insert into device_info_value values(21143,20553,5547,1,1,'is running.');
insert into device_info_value values(21144,20553,5547,2,2,'is ready.');
insert into device_info_value values(21145,20553,5547,3,3,'is terminated.');
insert into device_info_value values(21146,20554,5548,0,0,'is halted.');
insert into device_info_value values(21147,20554,5548,1,1,'is running.');
insert into device_info_value values(21148,20554,5548,2,2,'is ready.');
insert into device_info_value values(21149,20554,5548,3,3,'is terminated.');
insert into device_info_value values(21150,20393,5507,NULL,NULL,NULL);
insert into device_info_value values(21151,20439,5515,NULL,NULL,NULL);
insert into device_info_value values(21152,20440,5519,NULL,NULL,NULL);
insert into device_info_value values(21153,20555,5521,NULL,NULL,NULL);
insert into device_info_value values(21154,20556,5522,NULL,NULL,NULL);
insert into device_info_value values(21155,20557,5520,NULL,NULL,NULL);
insert into device_info_value values(21156,20441,5533,NULL,NULL,NULL);
insert into device_info_value values(21157,20558,5516,0,0,'error detected.');
insert into device_info_value values(21158,20558,5517,1,1,'is ok.');
insert into device_info_value values(21159,20559,5516,0,0,'error detected.');
insert into device_info_value values(21160,20559,5517,1,1,'is ok.');
insert into device_info_value values(21161,20560,5516,0,0,'error detected.');
insert into device_info_value values(21162,20560,5517,1,1,'is ok.');
insert into device_info_value values(21163,20561,5516,0,0,'error detected.');
insert into device_info_value values(21164,20561,5517,1,1,'is ok.');
insert into device_info_value values(21165,20562,5516,0,0,'overflow detected.');
insert into device_info_value values(21166,20562,5517,1,1,'is ok.');
insert into device_info_value values(21167,20563,5516,0,0,'error detected.');
insert into device_info_value values(21168,20563,5517,1,1,'is ok.');
insert into device_info_value values(21169,20564,5516,0,0,'error detected.');
insert into device_info_value values(21170,20564,5517,1,1,'is ok.');
insert into device_info_value values(21171,20442,5524,NULL,NULL,NULL);
insert into device_info_value values(21172,20565,5525,0,0,'is disabled.');
insert into device_info_value values(21173,20565,5525,1,1,'is enabled.');
insert into device_info_value values(21174,20566,5526,0,0,'is disabled.');
insert into device_info_value values(21175,20566,5526,1,1,'is enabled.');
insert into device_info_value values(21176,20567,5527,0,0,'is disabled.');
insert into device_info_value values(21177,20567,5527,1,1,'is enabled.');
insert into device_info_value values(21178,20568,5528,0,0,'is disabled.');
insert into device_info_value values(21179,20568,5528,1,1,'is enabled.');
insert into device_info_value values(21180,20443,5523,NULL,NULL,NULL);
insert into device_info_value values(21181,20444,5555,NULL,NULL,NULL);
insert into device_info_value values(21182,20445,5552,NULL,NULL,NULL);
insert into device_info_value values(21183,20569,5551,1,1,'is normal');
insert into device_info_value values(21184,20569,5551,0,0,'is reduced.');
insert into device_info_value values(21185,20570,5554,1,1,'is deactivated.');
insert into device_info_value values(21186,20570,5554,0,0,'is activated.');
insert into device_info_value values(21187,20571,5553,3,3,'1200');
insert into device_info_value values(22650,22641,5507,NULL,NULL,NULL);
insert into device_info_value values(22651,22642,5523,NULL,NULL,NULL);
insert into device_info_value values(22652,22643,5552,NULL,NULL,NULL);
insert into device_info_value values(22653,22644,5555,NULL,NULL,NULL);
insert into device_info_value values(22654,22645,NULL,NULL,NULL,NULL);
insert into device_info_value values(22655,22646,NULL,NULL,NULL,NULL);
insert into device_info_value values(22656,22647,5551,1,1,'is normal');
insert into device_info_value values(22657,22647,5551,0,0,'is reduced');
insert into device_info_value values(22658,22648,5554,1,1,'is deactivated');
insert into device_info_value values(22659,22648,5554,0,0,'is activated');
insert into device_info_value values(22660,22649,5553,3,3,'1200');

commit;

#
#  Geräte-Info einfügen.
#
insert into device_info values (20386,500,NULL,'BCU 1 Powerline',NULL,NULL,NULL,2);
insert into device_info values (20387,501,NULL,'BCU 1 Powerline',NULL,NULL,NULL,2);
insert into device_info values (20388,498,NULL,'BIM M112',NULL,NULL,NULL,0);
insert into device_info values (20389,499,NULL,'BCU 2',NULL,NULL,NULL,2);
insert into device_info values (20390,17041,NULL,'Coupler',NULL,NULL,NULL,0);
insert into device_info values (20391,17042,NULL,'Coupler',NULL,NULL,NULL,0);
insert into device_info values (20392,20332,NULL,'BIM M112',NULL,NULL,NULL,0);
insert into device_info values (20393,20334,NULL,'BCU 1 Powerline',NULL,NULL,NULL,2);
insert into device_info values (20394,113,NULL,'BCU 1',NULL,NULL,NULL,2);
insert into device_info values (20395,250,NULL,'BCU 1',NULL,NULL,NULL,2);
insert into device_info values (20396,497,NULL,'BCU 1',NULL,NULL,NULL,2);
insert into device_info values (20397,497,20396,'Runtime errors',6,NULL,NULL,2);
insert into device_info values (20398,497,20396,'Mode',8,NULL,NULL,0);
insert into device_info values (20399,497,20396,'Manufacturer',10,4,NULL,2);
insert into device_info values (20400,500,20386,'Mask version',1,1,NULL,2);
insert into device_info values (20401,500,20386,'Adapter type',NULL,NULL,NULL,2);
insert into device_info values (20402,500,20386,'Runtime errors',6,NULL,NULL,2);
insert into device_info values (20403,500,20386,'Mode',8,NULL,NULL,0);
insert into device_info values (20404,500,20386,'Manufacturer',10,4,NULL,2);
insert into device_info values (20405,500,20386,'System ID',35,1,NULL,2);
insert into device_info values (20406,500,20386,'Configuration',11,7,NULL,2);
insert into device_info values (20407,501,20387,'Mask version',1,1,NULL,2);
insert into device_info values (20408,501,20387,'Adapter type',NULL,NULL,NULL,2);
insert into device_info values (20409,501,20387,'Runtime errors',6,NULL,NULL,2);
insert into device_info values (20410,501,20387,'Mode',8,NULL,NULL,0);
insert into device_info values (20411,501,20387,'Manufacturer',10,4,NULL,2);
insert into device_info values (20412,501,20387,'System ID',35,1,NULL,2);
insert into device_info values (20413,501,20387,'Configuration',11,7,NULL,2);
insert into device_info values (20414,498,20388,'Mask version',1,1,NULL,0);
insert into device_info values (20415,498,20388,'Safe state',13,1,NULL,0);
insert into device_info values (20416,498,20388,'Adapter type',NULL,NULL,NULL,0);
insert into device_info values (20417,498,20388,'Load state machine',NULL,NULL,NULL,0);
insert into device_info values (20418,498,20388,'Run state machine',NULL,NULL,NULL,0);
insert into device_info values (20419,499,20389,'Mask version',1,1,NULL,2);
insert into device_info values (20420,499,20389,'Adapter type',NULL,NULL,NULL,2);
insert into device_info values (20421,499,20389,'Runtime errors',6,NULL,NULL,2);
insert into device_info values (20422,499,20389,'Mode',8,NULL,NULL,0);
insert into device_info values (20423,499,20389,'Manufacturer',10,4,NULL,2);
insert into device_info values (20424,17041,20390,'Mask version',1,1,NULL,0);
insert into device_info values (20425,17041,20390,'Manufacturer',10,4,NULL,0);
insert into device_info values (20426,17041,20390,'Master',NULL,NULL,NULL,0);
insert into device_info values (20427,17041,20390,'Slave  ',NULL,NULL,NULL,0);
insert into device_info values (20428,17041,20390,'State',20,1000,4,0);
insert into device_info values (20429,17042,20391,'Mask version',1,1,NULL,0);
insert into device_info values (20430,17042,20391,'Manufacturer',10,4,NULL,0);
insert into device_info values (20431,17042,20391,'Master',NULL,NULL,NULL,0);
insert into device_info values (20432,17042,20391,'Slave  ',NULL,NULL,NULL,0);
insert into device_info values (20433,17042,20391,'State',20,1000,4,0);
insert into device_info values (20434,20332,20392,'Mask version',1,1,NULL,0);
insert into device_info values (20435,20332,20392,'Safe state',13,1,NULL,0);
insert into device_info values (20436,20332,20392,'Adapter type',NULL,NULL,NULL,0);
insert into device_info values (20437,20332,20392,'Load state machine',NULL,NULL,NULL,0);
insert into device_info values (20438,20332,20392,'Run state machine',NULL,NULL,NULL,0);
insert into device_info values (20439,20334,20393,'Mask version',1,1,NULL,2);
insert into device_info values (20440,20334,20393,'Adapter type',NULL,NULL,NULL,2);
insert into device_info values (20441,20334,20393,'Runtime errors',6,NULL,NULL,2);
insert into device_info values (20442,20334,20393,'Mode',8,NULL,NULL,0);
insert into device_info values (20443,20334,20393,'Manufacturer',10,4,NULL,2);
insert into device_info values (20444,20334,20393,'System ID',35,1,NULL,2);
insert into device_info values (20445,20334,20393,'Configuration',11,7,NULL,2);
insert into device_info values (20446,113,20394,'Mask version',1,1,NULL,2);
insert into device_info values (20447,113,20394,'Bus voltage',2,1,NULL,0);
insert into device_info values (20448,113,20394,'Adapter type',NULL,NULL,NULL,2);
insert into device_info values (20449,113,20394,'Runtime errors',6,NULL,NULL,2);
insert into device_info values (20450,113,20394,'Mode',8,NULL,NULL,0);
insert into device_info values (20451,113,20394,'Manufacturer',10,4,NULL,2);
insert into device_info values (20452,250,20395,'Mask version',1,1,NULL,2);
insert into device_info values (20453,250,20395,'Bus voltage',2,1,NULL,0);
insert into device_info values (20454,250,20395,'Adapter type',NULL,NULL,NULL,2);
insert into device_info values (20455,250,20395,'Runtime errors',6,NULL,NULL,2);
insert into device_info values (20456,250,20395,'Mode',8,NULL,NULL,0);
insert into device_info values (20457,250,20395,'Manufacturer',10,4,NULL,2);
insert into device_info values (20458,497,20396,'Mask version',1,1,NULL,2);
insert into device_info values (20459,497,20396,'Bus voltage',2,1,NULL,0);
insert into device_info values (20460,497,20396,'Adapter type',NULL,NULL,NULL,2);
insert into device_info values (20461,497,20460,'Real PEI name',5,4,NULL,0);
insert into device_info values (20462,497,20397,'Internal system (0)',7,7,0,2);
insert into device_info values (20463,497,20397,'Internal system (1)',7,7,1,2);
insert into device_info values (20464,497,20397,'Internal system (2)',7,7,5,2);
insert into device_info values (20465,497,20397,'Internal system (3)',7,7,6,2);
insert into device_info values (20466,497,20397,'Stack',7,7,3,2);
insert into device_info values (20467,497,20397,'Communication object/Association table',7,7,4,2);
insert into device_info values (20468,497,20397,'EEPROM check',7,7,2,2);
insert into device_info values (20469,497,20398,'Serial PEI interface',9,7,4,0);
insert into device_info values (20470,497,20398,'User program',9,7,5,0);
insert into device_info values (20471,497,20398,'Download mode',9,7,6,0);
insert into device_info values (20472,497,20398,'Programming mode',9,7,0,0);
insert into device_info values (20473,500,20401,'Projected PEI number',3,1,NULL,2);
insert into device_info values (20474,500,20401,'Real PEI number',4,1,NULL,0);
insert into device_info values (20475,500,20401,'Real PEI name',5,4,NULL,0);
insert into device_info values (20476,500,20402,'Internal system (0)',7,7,0,2);
insert into device_info values (20477,500,20402,'Internal system (1)',7,7,1,2);
insert into device_info values (20478,500,20402,'Internal system (2)',7,7,5,2);
insert into device_info values (20479,500,20402,'Internal system (3)',7,7,6,2);
insert into device_info values (20480,500,20402,'Stack',7,7,3,2);
insert into device_info values (20481,500,20402,'Communication object/Association table',7,7,4,2);
insert into device_info values (20482,500,20402,'EEPROM check',7,7,2,2);
insert into device_info values (20483,500,20403,'Serial PEI interface',9,7,4,0);
insert into device_info values (20484,500,20403,'User program',9,7,5,0);
insert into device_info values (20485,500,20403,'Download mode',9,7,6,0);
insert into device_info values (20486,500,20403,'Programming mode',9,7,0,0);
insert into device_info values (20487,500,20406,'Sensitivity',12,7,7,2);
insert into device_info values (20488,500,20406,'Repeater mode',12,7,4,2);
insert into device_info values (20489,500,20406,'Baud rate',12,1000,2,2);
insert into device_info values (20490,501,20408,'Projected PEI number',3,1,NULL,2);
insert into device_info values (20491,501,20408,'Real PEI number',4,1,NULL,0);
insert into device_info values (20492,501,20408,'Real PEI name',5,4,NULL,0);
insert into device_info values (20493,501,20409,'Internal system (0)',7,7,0,2);
insert into device_info values (20494,501,20409,'Internal system (1)',7,7,1,2);
insert into device_info values (20495,501,20409,'Internal system (2)',7,7,5,2);
insert into device_info values (20496,501,20409,'Internal system (3)',7,7,6,2);
insert into device_info values (20497,501,20409,'Stack',7,7,3,2);
insert into device_info values (20498,501,20409,'Communication object/Association table',7,7,4,2);
insert into device_info values (20499,501,20409,'EEPROM check',7,7,2,2);
insert into device_info values (20500,501,20410,'Serial PEI interface',9,7,4,0);
insert into device_info values (20501,501,20410,'User program',9,7,5,0);
insert into device_info values (20502,501,20410,'Download mode',9,7,6,0);
insert into device_info values (20503,501,20410,'Programming mode',9,7,0,0);
insert into device_info values (20504,501,20413,'Sensitivity',12,7,7,2);
insert into device_info values (20505,501,20413,'Repeater mode',12,7,4,2);
insert into device_info values (20506,501,20413,'Baud rate',12,1000,2,2);
insert into device_info values (20507,498,20416,'Projected PEI number',3,1,NULL,0);
insert into device_info values (20508,498,20416,'Real PEI number',4,1,NULL,0);
insert into device_info values (20509,498,20416,'Real PEI name',5,4,NULL,0);
insert into device_info values (20510,498,20417,'Address table',14,1,NULL,0);
insert into device_info values (20511,498,20417,'Association table',15,1,NULL,0);
insert into device_info values (20512,498,20417,'Application program',16,1,NULL,0);
insert into device_info values (20513,498,20417,'PEI program',17,1,NULL,0);
insert into device_info values (20514,498,20418,'Application  program',18,1,NULL,0);
insert into device_info values (20515,498,20418,'PEI  program',19,1,NULL,0);
insert into device_info values (20516,499,20420,'Projected PEI number',3,1,NULL,2);
insert into device_info values (20517,499,20420,'Real PEI number',4,1,NULL,0);
insert into device_info values (20518,499,20420,'Real PEI name',5,4,NULL,0);
insert into device_info values (20519,499,20421,'Internal system (0)',7,7,0,2);
insert into device_info values (20520,499,20421,'Internal system (1)',7,7,1,2);
insert into device_info values (20521,499,20421,'Internal system (2)',7,7,5,2);
insert into device_info values (20522,499,20421,'Internal system (3)',7,7,6,2);
insert into device_info values (20523,499,20421,'Stack',7,7,3,2);
insert into device_info values (20524,499,20421,'Communication object/Association table',7,7,4,2);
insert into device_info values (20525,499,20421,'EEPROM check',7,7,2,2);
insert into device_info values (20526,499,20422,'Serial PEI interface',9,7,4,0);
insert into device_info values (20527,499,20422,'User program',9,7,5,0);
insert into device_info values (20528,499,20422,'Download mode',9,7,6,0);
insert into device_info values (20529,499,20422,'Programming mode',9,7,0,0);
insert into device_info values (20530,17041,20426,'Definitions ',21,NULL,NULL,0);
insert into device_info values (20531,17041,20427,'Definitions',23,NULL,NULL,0);
insert into device_info values (20532,17041,20426,'Group configuration ',25,NULL,NULL,0);
insert into device_info values (20533,17041,20427,'Group configuration',27,NULL,NULL,0);
insert into device_info values (20534,17041,20426,'Error(s) ',29,1,NULL,0);
insert into device_info values (20535,17041,20427,'Error(s)',31,1,NULL,0);
insert into device_info values (20536,17041,20426,'Runtime errors ',6,NULL,NULL,0);
insert into device_info values (20537,17041,20427,'Runtime errors',33,NULL,NULL,0);
insert into device_info values (20538,17042,20431,'Definitions ',21,NULL,NULL,0);
insert into device_info values (20539,17042,20432,'Definitions',23,NULL,NULL,0);
insert into device_info values (20540,17042,20431,'Group configuration ',25,NULL,NULL,0);
insert into device_info values (20541,17042,20432,'Group configuration',27,NULL,NULL,0);
insert into device_info values (20542,17042,20431,'Error(s) ',29,1,NULL,0);
insert into device_info values (20543,17042,20432,'Error(s)',31,1,NULL,0);
insert into device_info values (20544,17042,20431,'Runtime errors ',6,NULL,NULL,0);
insert into device_info values (20545,17042,20432,'Runtime errors',33,NULL,NULL,0);
insert into device_info values (20546,20332,20436,'Projected PEI number',3,1,NULL,0);
insert into device_info values (20547,20332,20436,'Real PEI number',4,1,NULL,0);
insert into device_info values (20548,20332,20436,'Real PEI name',5,4,NULL,0);
insert into device_info values (20549,20332,20437,'Address table',14,1,NULL,0);
insert into device_info values (20550,20332,20437,'Association table',15,1,NULL,0);
insert into device_info values (20551,20332,20437,'Application program',16,1,NULL,0);
insert into device_info values (20552,20332,20437,'PEI program',17,1,NULL,0);
insert into device_info values (20553,20332,20438,'Application  program',18,1,NULL,0);
insert into device_info values (20554,20332,20438,'PEI  program',19,1,NULL,0);
insert into device_info values (20555,20334,20440,'Projected PEI number',3,1,NULL,2);
insert into device_info values (20556,20334,20440,'Real PEI number',4,1,NULL,0);
insert into device_info values (20557,20334,20440,'Real PEI name',5,4,NULL,0);
insert into device_info values (20558,20334,20441,'Internal system (0)',7,7,0,2);
insert into device_info values (20559,20334,20441,'Internal system (1)',7,7,1,2);
insert into device_info values (20560,20334,20441,'Internal system (2)',7,7,5,2);
insert into device_info values (20561,20334,20441,'Internal system (3)',7,7,6,2);
insert into device_info values (20562,20334,20441,'Stack',7,7,3,2);
insert into device_info values (20563,20334,20441,'Communication object/Association table',7,7,4,2);
insert into device_info values (20564,20334,20441,'EEPROM check',7,7,2,2);
insert into device_info values (20565,20334,20442,'Serial PEI interface',9,7,4,0);
insert into device_info values (20566,20334,20442,'User program',9,7,5,0);
insert into device_info values (20567,20334,20442,'Download mode',9,7,6,0);
insert into device_info values (20568,20334,20442,'Programming mode',9,7,0,0);
insert into device_info values (20569,20334,20445,'Sensitivity',12,7,7,2);
insert into device_info values (20570,20334,20445,'Repeater mode',12,7,4,2);
insert into device_info values (20571,20334,20445,'Baud rate',12,1000,2,2);
insert into device_info values (20572,113,20448,'Projected PEI number',3,1,NULL,2);
insert into device_info values (20573,113,20448,'Real PEI number',4,1,NULL,0);
insert into device_info values (20574,113,20448,'Real PEI name',5,4,NULL,0);
insert into device_info values (20575,113,20449,'Internal system (0)',7,7,0,2);
insert into device_info values (20576,113,20449,'Internal system (1)',7,7,1,2);
insert into device_info values (20577,113,20449,'Internal system (2)',7,7,5,2);
insert into device_info values (20578,113,20449,'Internal system (3)',7,7,6,2);
insert into device_info values (20579,113,20449,'Stack',7,7,3,2);
insert into device_info values (20580,113,20449,'Communication object/Association table',7,7,4,2);
insert into device_info values (20581,113,20449,'EEPROM check',7,7,2,2);
insert into device_info values (20582,113,20450,'Serial PEI interface',9,7,4,0);
insert into device_info values (20583,113,20450,'User program',9,7,5,0);
insert into device_info values (20584,113,20450,'Download mode',9,7,6,0);
insert into device_info values (20585,113,20450,'Programming mode',9,7,0,0);
insert into device_info values (20586,250,20454,'Projected PEI number',3,1,NULL,2);
insert into device_info values (20587,250,20454,'Real PEI number',4,1,NULL,0);
insert into device_info values (20588,250,20454,'Real PEI name',5,4,NULL,0);
insert into device_info values (20589,250,20455,'Internal system (0)',7,7,0,2);
insert into device_info values (20590,250,20455,'Internal system (1)',7,7,1,2);
insert into device_info values (20591,250,20455,'Internal system (2)',7,7,5,2);
insert into device_info values (20592,250,20455,'Internal system (3)',7,7,6,2);
insert into device_info values (20593,250,20455,'Stack',7,7,3,2);
insert into device_info values (20594,250,20455,'Communication object/Association table',7,7,4,2);
insert into device_info values (20595,250,20455,'EEPROM check',7,7,2,2);
insert into device_info values (20596,250,20456,'Serial PEI interface',9,7,4,0);
insert into device_info values (20597,250,20456,'User program',9,7,5,0);
insert into device_info values (20598,250,20456,'Download mode',9,7,6,0);
insert into device_info values (20599,250,20456,'Programming mode',9,7,0,0);
insert into device_info values (20600,497,20460,'Projected PEI number',3,1,NULL,2);
insert into device_info values (20601,497,20460,'Real PEI number',4,1,NULL,0);
insert into device_info values (20602,17041,20536,'Internal system (0) ',7,7,0,0);
insert into device_info values (20603,17041,20536,'Internal system (1) ',7,7,1,0);
insert into device_info values (20604,17041,20536,'Internal system (2) ',7,7,5,0);
insert into device_info values (20605,17041,20536,'Internal system (3) ',7,7,6,0);
insert into device_info values (20606,17041,20536,'Stack ',7,7,3,0);
insert into device_info values (20607,17041,20536,'Communication object/Association table ',7,7,4,0);
insert into device_info values (20608,17041,20536,'EEPROM check ',7,7,2,0);
insert into device_info values (20609,17041,20537,'Internal system (0)',34,7,0,0);
insert into device_info values (20610,17041,20537,'Internal system (1)',34,7,1,0);
insert into device_info values (20611,17041,20537,'Internal system (2)',34,7,5,0);
insert into device_info values (20612,17041,20537,'Internal system (3)',34,7,6,0);
insert into device_info values (20613,17041,20537,'Stack',34,7,3,0);
insert into device_info values (20614,17041,20537,'Communication object/Association table',34,7,4,0);
insert into device_info values (20615,17041,20537,'EEPROM check',34,7,2,0);
insert into device_info values (20616,17041,20530,'Group table ',22,1005,1,0);
insert into device_info values (20617,17041,20530,'Group addresses ',22,1004,1,0);
insert into device_info values (20618,17041,20530,'Mainline:   ',22,1007,1,0);
insert into device_info values (20619,17041,20530,'Subline:   ',22,1006,1,0);
insert into device_info values (20620,17041,20530,'Mainline:  ',22,1003,1,0);
insert into device_info values (20621,17041,20530,'Subline:  ',22,1002,1,0);
insert into device_info values (20622,17041,20535,'Group table',32,1001,1,0);
insert into device_info values (20623,17041,20531,'Group addresses',24,1004,1,0);
insert into device_info values (20624,17041,20531,'Mainline: ',24,1007,1,0);
insert into device_info values (20625,17041,20531,'Subline: ',24,1006,1,0);
insert into device_info values (20626,17041,20531,'Mainline:',24,1003,1,0);
insert into device_info values (20627,17041,20531,'Subline:',24,1002,1,0);
insert into device_info values (20628,17041,20532,'Main- to subline: ',26,1000,2,0);
insert into device_info values (20629,17041,20532,'Sub- to mainline: ',26,1004,2,0);
insert into device_info values (20630,17041,20533,'Main- to subline:',28,1000,2,0);
insert into device_info values (20631,17041,20533,'Sub- to mainline:',28,1004,2,0);
insert into device_info values (20632,17042,20544,'Internal system (0) ',7,7,0,0);
insert into device_info values (20633,17042,20544,'Internal system (1) ',7,7,1,0);
insert into device_info values (20634,17042,20544,'Internal system (2) ',7,7,5,0);
insert into device_info values (20635,17042,20544,'Internal system (3) ',7,7,6,0);
insert into device_info values (20636,17042,20544,'Stack ',7,7,3,0);
insert into device_info values (20637,17042,20544,'Communication object/Association table ',7,7,4,0);
insert into device_info values (20638,17042,20544,'EEPROM check ',7,7,2,0);
insert into device_info values (20639,17042,20545,'Internal system (0)',34,7,0,0);
insert into device_info values (20640,17042,20545,'Internal system (1)',34,7,1,0);
insert into device_info values (20641,17042,20545,'Internal system (2)',34,7,5,0);
insert into device_info values (20642,17042,20545,'Internal system (3)',34,7,6,0);
insert into device_info values (20643,17042,20545,'Stack',34,7,3,0);
insert into device_info values (20644,17042,20545,'Communication object/Association table',34,7,4,0);
insert into device_info values (20645,17042,20545,'EEPROM check',34,7,2,0);
insert into device_info values (20646,17042,20538,'Group table ',22,1005,1,0);
insert into device_info values (20647,17042,20538,'Group addresses ',22,1004,1,0);
insert into device_info values (20648,17042,20538,'Mainline:   ',22,1007,1,0);
insert into device_info values (20649,17042,20538,'Subline:   ',22,1006,1,0);
insert into device_info values (20650,17042,20538,'Mainline:  ',22,1003,1,0);
insert into device_info values (20651,17042,20538,'Subline:  ',22,1002,1,0);
insert into device_info values (20652,17042,20543,'Group table',32,1001,1,0);
insert into device_info values (20653,17042,20539,'Group addresses',24,1004,1,0);
insert into device_info values (20654,17042,20539,'Mainline: ',24,1007,1,0);
insert into device_info values (20655,17042,20539,'Subline: ',24,1006,1,0);
insert into device_info values (20656,17042,20539,'Mainline:',24,1003,1,0);
insert into device_info values (20657,17042,20539,'Subline:',24,1002,1,0);
insert into device_info values (20658,17042,20540,'Main- to subline: ',26,1000,2,0);
insert into device_info values (20659,17042,20540,'Sub- to mainline: ',26,1004,2,0);
insert into device_info values (20660,17042,20541,'Main- to subline:',28,1000,2,0);
insert into device_info values (20661,17042,20541,'Sub- to mainline:',28,1004,2,0);
insert into device_info values (20662,17041,20535,'Telegram length',32,1000,1,0);
insert into device_info values (20663,17041,20534,'Slave ',30,1002,1,0);
insert into device_info values (20664,17041,20534,'Telegram at TX-buffer',30,1003,1,0);
insert into device_info values (20665,17041,20535,'Connection of processors',32,1004,1,0);
insert into device_info values (20666,17041,20535,'Slave',32,1002,1,0);
insert into device_info values (20667,17041,20535,'Telegram',32,1003,1,0);
insert into device_info values (20668,17042,20543,'Telegram length',32,1000,1,0);
insert into device_info values (20669,17042,20542,'Slave ',30,1002,1,0);
insert into device_info values (20670,17042,20543,'Telegram at TX-buffer',32,1003,1,0);
insert into device_info values (20671,17042,20543,'Connection of processors',32,1004,1,0);
insert into device_info values (20672,17042,20543,'Slave',32,1002,1,0);
insert into device_info values (22641,22640,NULL,'Media Coupler',NULL,NULL,NULL,0);
insert into device_info values (22642,22640,22641,'Manufacturer',36,4,NULL,0);
insert into device_info values (22643,22640,22641,'Configuration',37,7,NULL,0);
insert into device_info values (22644,22640,22641,'System ID',39,1,NULL,0);
insert into device_info values (22645,22640,22641,'Device Type',40,1,NULL,0);
insert into device_info values (22646,22640,22641,'Program Version',41,1,NULL,0);
insert into device_info values (22647,22640,22643,'Sensitivity',38,7,7,0);
insert into device_info values (22648,22640,22643,'Repeater mode',38,7,4,0);
insert into device_info values (22649,22640,22643,'Baud rate',38,1000,2,0);

commit;


#
#  ETE-Sprachen einfügen.
#
insert into ete_language values(1025,'Arabic',0);
insert into ete_language values(1026,'Bulgarian',0);
insert into ete_language values(1027,'Catalan',0);
insert into ete_language values(1028,'Chinese',0);
insert into ete_language values(1029,'Czech',0);
insert into ete_language values(1030,'Danish',0);
insert into ete_language values(1031,'German',1);
insert into ete_language values(1032,'Greek',0);
insert into ete_language values(1033,'English',0);
insert into ete_language values(1034,'Spanish',0);
insert into ete_language values(1035,'Finnish',0);
insert into ete_language values(1036,'French',0);
insert into ete_language values(1037,'Hebrew',0);
insert into ete_language values(1038,'Hungarian',0);
insert into ete_language values(1039,'Icelandic',0);
insert into ete_language values(1040,'Italian',0);
insert into ete_language values(1041,'Japanese',0);
insert into ete_language values(1042,'Korean',0);
insert into ete_language values(1043,'Dutch',0);
insert into ete_language values(1044,'Norwegian',0);
insert into ete_language values(1045,'Polish',0);
insert into ete_language values(1046,'Braz. Portuguese',0);
insert into ete_language values(1047,'Rhaeto-Romanic',0);
insert into ete_language values(1048,'Romanian',0);
insert into ete_language values(1049,'Russian',0);
insert into ete_language values(1050,'Serbian',0);
insert into ete_language values(1051,'Slovak',0);
insert into ete_language values(1052,'Albanian',0);
insert into ete_language values(1053,'Swedish',0);
insert into ete_language values(1054,'Thai',0);
insert into ete_language values(1055,'Turkian',0);
insert into ete_language values(1056,'Urdu',0);
insert into ete_language values(1057,'Bahasa',0);
insert into ete_language values(2052,'Simple Chinese',0);
insert into ete_language values(2055,'Swiss German',0);
insert into ete_language values(2057,'UK English',0);
insert into ete_language values(2058,'Mexic. Spanish',0);
insert into ete_language values(2060,'Belg. French',0);
insert into ete_language values(2064,'Swiss Italian',0);
insert into ete_language values(2067,'Belg. Dutch',0);
insert into ete_language values(2068,'Norwegian (Nynorsk)',0);
insert into ete_language values(2070,'Portuguese',0);
insert into ete_language values(2074,'Croatian',0);
insert into ete_language values(3084,'Can. French',0);
insert into ete_language values(4108,'Swiss French',0);

commit;

#
#  IMEX-Files einfügen.
#
insert into imex_files values(11,'hw_product','product_picture');
insert into imex_files values(12,'hw_product','product_dll');
insert into imex_files values(13,'catalog_entry','entry_picture');
insert into imex_files values(14,'catalog_entry','help_file_name');
insert into imex_files values(15,'application_program','help_file_name');
insert into imex_files values(16,'functional_entity','help_file_name');
insert into imex_files values(17,'virtual_device','help_file_name');
insert into imex_files values(18,'additional_file','additional_file_name');
insert into imex_files values(19,'specification_text','spec_text_filename');
insert into imex_files values(20,'help_file','help_file_name');

commit;

#
#  IMEX-FS einfügen.
#

insert into imex_fs values(366,0,'manufacturer','manufacturer_compatibility','manufacturer_id','manufacturer_id','','manufacturer');
insert into imex_fs values(367,0,'manufacturer','manufacturer_compatibility','manufacturer_id','man_manufacturer_id','','manufacturer');
insert into imex_fs values(368,1,'address_format','address','address_format_id','address_format_id','','project');
insert into imex_fs values(369,1,'address','person','address_id','address_id','','project');
insert into imex_fs values(370,1,'person','project','person_id','project_responsible_id','','project');
insert into imex_fs values(371,1,'address','project','address_id','customer_address_id','','project');
insert into imex_fs values(372,1,'address','project','address_id','project_address_id','','project');
insert into imex_fs values(373,1,'ete_language','text_attribute','language_id','language_id','','project');
insert into imex_fs values(374,1,'ete_language','help_file','language_id','language_id','','project');
insert into imex_fs values(375,0,'project','building','project_id','project_id','bui_building_id is null','project');
insert into imex_fs values(376,0,'building','building','building_id','bui_building_id','','project');
insert into imex_fs values(377,0,'building','room','building_id','building_id','','project');
insert into imex_fs values(378,0,'room','device','room_id','room_id','','project');
insert into imex_fs values(379,0,'project','function','project_id','project_id','fun_function_id is null','project');
insert into imex_fs values(380,0,'function','function','function_id','fun_function_id','','project');
insert into imex_fs values(381,0,'function','device','function_id','function_id','','project');
insert into imex_fs values(382,0,'project','area','project_id','project_id','','project');
insert into imex_fs values(383,0,'area','line','area_id','area_id','','project');
insert into imex_fs values(384,0,'area','line_filter_table','area_id','area_id','','project');
insert into imex_fs values(385,0,'line','device','line_id','line_id','','project');
insert into imex_fs values(386,0,'line','line_filter_table','line_id','line_id','','project');
insert into imex_fs values(387,0,'line','polling_group','line_id','line_id','','project');
insert into imex_fs values(388,0,'polling_group','device_to_polling_group','polling_group_id','polling_group_id','','project');
insert into imex_fs values(389,1,'medium_type','line','medium_type_number','medium_type_number','','project');
insert into imex_fs values(390,0,'project','maingroup','project_id','project_id','','project');
insert into imex_fs values(391,0,'maingroup','middlegroup','maingroup_id','maingroup_id','','project');
insert into imex_fs values(392,0,'middlegroup','subgroup','middlegroup_id','middlegroup_id','','project');
insert into imex_fs values(393,0,'subgroup','subgroup_to_object','subgroup_id','subgroup_id','','project');
insert into imex_fs values(394,0,'subgroup','line_filter_table','subgroup_id','subgroup_id','','project');
insert into imex_fs values(395,0,'project','device','project_id','project_id','','project');
insert into imex_fs values(396,0,'project','project_config','project_id','project_id','','project');
insert into imex_fs values(397,0,'project','project_history','project_id','project_id','','project');
insert into imex_fs values(398,0,'project','additional_file','project_id','project_id','','project');
insert into imex_fs values(399,0,'project','system_id_lookup','project_id','project_id','','project');
insert into imex_fs values(400,0,'project','sysid_scan','project_id','project_id','','project');
insert into imex_fs values(401,0,'device','device_programming','device_id','device_id','','project');
insert into imex_fs values(402,0,'device','device_accessory','device_id','device_id','','project');
insert into imex_fs values(403,0,'device','device_parameter','device_id','device_id','','project');
insert into imex_fs values(404,0,'device','device_object','device_id','device_id','','project');
insert into imex_fs values(405,0,'device_object','subgroup_to_object','device_object_id','device_object_id','','project');
insert into imex_fs values(406,0,'device','device_to_polling_group','device_id','device_id','','project');
insert into imex_fs values(407,1,'catalog_entry','device','catalog_entry_id','catalog_entry_id','','project');
insert into imex_fs values(408,1,'catalog_entry','device_accessory','catalog_entry_id','catalog_entry_id','','project');
insert into imex_fs values(409,1,'hw_product','catalog_entry','product_id','product_id','','project');
insert into imex_fs values(410,1,'bcu_type','hw_product','bcu_type_number','bcu_type_number','','project');
insert into imex_fs values(411,1,'application_program','device','program_id','program_id','','project');
insert into imex_fs values(412,1,'application_program','device','program_id','pei_program_id','','project');
insert into imex_fs values(413,1,'manufacturer','hw_product','manufacturer_id','manufacturer_id','','project');
insert into imex_fs values(414,1,'manufacturer','hw_product','manufacturer_id','original_manufacturer_id','','project');
insert into imex_fs values(415,1,'manufacturer','application_program','manufacturer_id','manufacturer_id','','project');
insert into imex_fs values(416,1,'manufacturer','application_program','manufacturer_id','original_manufacturer_id','','project');
insert into imex_fs values(417,1,'manufacturer','catalog_entry','manufacturer_id','manufacturer_id','','project');
insert into imex_fs values(418,1,'mask','application_program','mask_id','mask_id','','project');
insert into imex_fs values(419,1,'assembler','application_program','assembler_id','assembler_id','','project');
insert into imex_fs values(420,1,'symbol','application_program','symbol_id','symbol_id','','project');
insert into imex_fs values(421,1,'symbol','symbol_condition','symbol_id','symbol_id','','project');
insert into imex_fs values(422,1,'symbol','catalog_entry','symbol_id','symbol_id','','project');
insert into imex_fs values(423,1,'symbol','hw_product','symbol_id','symbol_id','','project');
insert into imex_fs values(424,0,'mask','mask_entry','mask_id','mask_id','','project');
insert into imex_fs values(425,0,'mask','mask_system_pointer','mask_id','mask_id','','project');
insert into imex_fs values(426,0,'mask','mask_compatibility','mask_id','new_mask_id','','project');
insert into imex_fs values(427,1,'mask','mask_compatibility','mask_id','old_mask_id','','project');
insert into imex_fs values(428,0,'mask','device_info','mask_id','mask_id','dev_device_info_id IS NULL','project');
insert into imex_fs values(429,0,'device_info','text_attribute','device_info_id','entity_id',NULL,'project');
insert into imex_fs values(430,0,'device_info','device_info','device_info_id','dev_device_info_id','','project');
insert into imex_fs values(431,0,'device_info','device_info_value','device_info_id','device_info_id','','project');
insert into imex_fs values(432,0,'device_info_value','text_attribute','device_info_value_id','entity_id',NULL,'project');
insert into imex_fs values(433,0,'catalog_entry','product_description','catalog_entry_id','catalog_entry_id','','project');
insert into imex_fs values(434,1,'ete_language','product_description','language_id','language_id','','project');
insert into imex_fs values(435,0,'catalog_entry','accessory','catalog_entry_id','cat_catalog_entry_id','','project');
insert into imex_fs values(436,1,'catalog_entry','accessory','catalog_entry_id','catalog_entry_id','','project');
insert into imex_fs values(437,0,'catalog_entry','text_attribute','catalog_entry_id','entity_id','','project');
insert into imex_fs values(438,0,'catalog_entry','help_file','catalog_entry_id','entity_id','','project');
insert into imex_fs values(439,0,'catalog_entry','text_to_catent','catalog_entry_id','catalog_entry_id','','project');
insert into imex_fs values(440,1,'specification_text','text_to_catent','spec_text_id','spec_text_id','','project');
insert into imex_fs values(441,0,'hw_product','product_to_program','product_id','product_id','','project');
insert into imex_fs values(442,1,'application_program','product_to_program','program_id','program_id','','project');
insert into imex_fs values(443,1,'application_program','product_to_program','program_id','pei_program_id','','project');
insert into imex_fs values(444,0,'product_to_program','product_to_program_to_mt','prod2prog_id','prod2prog_id','','project');
insert into imex_fs values(445,1,'medium_type','product_to_program_to_mt','medium_type_number','medium_type_number','','project');
insert into imex_fs values(446,0,'application_program','text_attribute','program_id','entity_id','','project');
insert into imex_fs values(447,0,'application_program','help_file','program_id','entity_id','','project');
insert into imex_fs values(448,0,'application_program','program_description','program_id','program_id','','project');
insert into imex_fs values(449,1,'ete_language','program_description','language_id','language_id','','project');
insert into imex_fs values(450,0,'application_program','s19_block','program_id','program_id','','project');
insert into imex_fs values(451,0,'s19_block','s19_block_paragraph','block_id','block_id','','project');
insert into imex_fs values(452,0,'application_program','communication_object','program_id','program_id','','project');
insert into imex_fs values(453,0,'application_program','address_fixup','program_id','program_id','','project');
insert into imex_fs values(454,0,'application_program','parameter_type','program_id','program_id','','project');
insert into imex_fs values(455,0,'application_program','parameter','program_id','program_id','par_parameter_id is null','project');
insert into imex_fs values(456,0,'parameter_type','parameter','parameter_type_id','parameter_type_id','par_parameter_id is null','project');
insert into imex_fs values(457,0,'parameter_type','parameter_list_of_values','parameter_type_id','parameter_type_id','','project');
insert into imex_fs values(458,0,'parameter_list_of_values','text_attribute','parameter_value_id','entity_id','','project');
insert into imex_fs values(459,0,'parameter','parameter','parameter_id','par_parameter_id','','project');
insert into imex_fs values(460,0,'parameter','communication_object','parameter_id','parameter_id','','project');
insert into imex_fs values(461,0,'parameter','symbol_condition','parameter_id','parameter_id','','project');
insert into imex_fs values(462,0,'parameter','text_attribute','parameter_id','entity_id','','project');
insert into imex_fs values(463,1,'parameter_atomic_type','parameter_type','atomic_type_number','atomic_type_number','','project');
insert into imex_fs values(464,0,'communication_object','text_attribute','object_id','entity_id','','project');
insert into imex_fs values(465,1,'object_type','communication_object','object_type_code','object_type','','project');
insert into imex_fs values(466,1,'object_priority','communication_object','object_priority_code','object_priority','','project');
insert into imex_fs values(467,1,'manufacturer','application_program','manufacturer_id','manufacturer_id','','application_program');
insert into imex_fs values(468,1,'manufacturer','application_program','manufacturer_id','original_manufacturer_id','','application_program');
insert into imex_fs values(469,1,'mask','application_program','mask_id','mask_id','','application_program');
insert into imex_fs values(470,1,'assembler','application_program','assembler_id','assembler_id','','application_program');
insert into imex_fs values(471,1,'symbol','application_program','symbol_id','symbol_id','','application_program');
insert into imex_fs values(472,1,'symbol','symbol_condition','symbol_id','symbol_id','','application_program');
insert into imex_fs values(473,1,'ete_language','text_attribute','language_id','language_id','','application_program');
insert into imex_fs values(474,1,'ete_language','help_file','language_id','language_id','','application_program');
insert into imex_fs values(475,0,'mask','mask_entry','mask_id','mask_id','','application_program');
insert into imex_fs values(476,0,'mask','mask_system_pointer','mask_id','mask_id','','application_program');
insert into imex_fs values(477,0,'mask','mask_compatibility','mask_id','new_mask_id','','application_program');
insert into imex_fs values(478,1,'mask','mask_compatibility','mask_id','old_mask_id','','application_program');
insert into imex_fs values(479,0,'mask','device_info','mask_id','mask_id','dev_device_info_id IS NULL','application_program');
insert into imex_fs values(480,0,'device_info','text_attribute','device_info_id','entity_id',NULL,'application_program');
insert into imex_fs values(481,0,'device_info','device_info','device_info_id','dev_device_info_id','','application_program');
insert into imex_fs values(482,0,'device_info','device_info_value','device_info_id','device_info_id','','application_program');
insert into imex_fs values(483,0,'device_info_value','text_attribute','device_info_value_id','entity_id',NULL,'application_program');
insert into imex_fs values(484,0,'application_program','text_attribute','program_id','entity_id','','application_program');
insert into imex_fs values(485,0,'application_program','help_file','program_id','entity_id','','application_program');
insert into imex_fs values(486,0,'application_program','program_description','program_id','program_id','','application_program');
insert into imex_fs values(487,1,'ete_language','program_description','language_id','language_id','','application_program');
insert into imex_fs values(488,0,'application_program','s19_block','program_id','program_id','','application_program');
insert into imex_fs values(489,0,'s19_block','s19_block_paragraph','block_id','block_id','','application_program');
insert into imex_fs values(490,0,'application_program','communication_object','program_id','program_id','','application_program');
insert into imex_fs values(491,0,'application_program','address_fixup','program_id','program_id','','application_program');
insert into imex_fs values(492,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
insert into imex_fs values(493,0,'application_program','parameter_type','program_id','program_id','','application_program');
insert into imex_fs values(494,0,'application_program','parameter','program_id','program_id','par_parameter_id is null','application_program');
insert into imex_fs values(495,0,'parameter_type','parameter','parameter_type_id','parameter_type_id','par_parameter_id is null','application_program');
insert into imex_fs values(496,0,'parameter_type','parameter_list_of_values','parameter_type_id','parameter_type_id','','application_program');
insert into imex_fs values(497,0,'parameter_list_of_values','text_attribute','parameter_value_id','entity_id','','application_program');
insert into imex_fs values(498,0,'parameter','parameter','parameter_id','par_parameter_id','','application_program');
insert into imex_fs values(499,0,'parameter','communication_object','parameter_id','parameter_id','','application_program');
insert into imex_fs values(500,0,'parameter','symbol_condition','parameter_id','parameter_id','','application_program');
insert into imex_fs values(501,0,'parameter','text_attribute','parameter_id','entity_id','','application_program');
insert into imex_fs values(502,0,'communication_object','text_attribute','object_id','entity_id','','application_program');
insert into imex_fs values(503,1,'object_type','communication_object','object_type_code','object_type','','application_program');
insert into imex_fs values(504,1,'object_priority','communication_object','object_priority_code','object_priority','','application_program');
insert into imex_fs values(505,1,'parameter_atomic_type','parameter_type','atomic_type_number','atomic_type_number','','application_program');
insert into imex_fs values(506,1,'bcu_type','hw_product','bcu_type_number','bcu_type_number','','hw_product');
insert into imex_fs values(507,1,'manufacturer','hw_product','manufacturer_id','manufacturer_id','','hw_product');
insert into imex_fs values(508,1,'manufacturer','hw_product','manufacturer_id','original_manufacturer_id','','hw_product');
insert into imex_fs values(509,1,'manufacturer','application_program','manufacturer_id','manufacturer_id','','hw_product');
insert into imex_fs values(510,1,'manufacturer','application_program','manufacturer_id','original_manufacturer_id','','hw_product');
insert into imex_fs values(511,1,'manufacturer','catalog_entry','manufacturer_id','manufacturer_id','','hw_product');
insert into imex_fs values(512,1,'mask','application_program','mask_id','mask_id','','hw_product');
insert into imex_fs values(513,1,'assembler','application_program','assembler_id','assembler_id','','hw_product');
insert into imex_fs values(514,1,'symbol','application_program','symbol_id','symbol_id','','hw_product');
insert into imex_fs values(515,1,'symbol','symbol_condition','symbol_id','symbol_id','','hw_product');
insert into imex_fs values(516,1,'symbol','catalog_entry','symbol_id','symbol_id','','hw_product');
insert into imex_fs values(517,1,'symbol','hw_product','symbol_id','symbol_id','','hw_product');
insert into imex_fs values(518,1,'ete_language','text_attribute','language_id','language_id','','hw_product');
insert into imex_fs values(519,1,'ete_language','help_file','language_id','language_id','','hw_product');
insert into imex_fs values(520,0,'mask','mask_entry','mask_id','mask_id','','hw_product');
insert into imex_fs values(521,0,'mask','mask_system_pointer','mask_id','mask_id','','hw_product');
insert into imex_fs values(522,0,'mask','mask_compatibility','mask_id','new_mask_id','','hw_product');
insert into imex_fs values(523,1,'mask','mask_compatibility','mask_id','old_mask_id','','hw_product');
insert into imex_fs values(524,0,'mask','device_info','mask_id','mask_id','dev_device_info_id IS NULL','hw_product');
insert into imex_fs values(525,0,'device_info','text_attribute','device_info_id','entity_id',NULL,'hw_product');
insert into imex_fs values(526,0,'device_info','device_info','device_info_id','dev_device_info_id','','hw_product');
insert into imex_fs values(527,0,'device_info','device_info_value','device_info_id','device_info_id','','hw_product');
insert into imex_fs values(528,0,'device_info_value','text_attribute','device_info_value_id','entity_id',NULL,'hw_product');
insert into imex_fs values(529,0,'catalog_entry','product_description','catalog_entry_id','catalog_entry_id','','hw_product');
insert into imex_fs values(530,1,'ete_language','product_description','language_id','language_id','','hw_product');
insert into imex_fs values(531,0,'catalog_entry','accessory','catalog_entry_id','cat_catalog_entry_id','','hw_product');
insert into imex_fs values(532,1,'catalog_entry','accessory','catalog_entry_id','catalog_entry_id','','hw_product');
insert into imex_fs values(533,0,'catalog_entry','text_attribute','catalog_entry_id','entity_id','','hw_product');
insert into imex_fs values(534,0,'catalog_entry','help_file','catalog_entry_id','entity_id','','hw_product');
insert into imex_fs values(535,0,'catalog_entry','text_to_catent','catalog_entry_id','catalog_entry_id','','hw_product');
insert into imex_fs values(536,1,'specification_text','text_to_catent','spec_text_id','spec_text_id','','hw_product');
insert into imex_fs values(537,0,'hw_product','catalog_entry','product_id','product_id','','hw_product');
insert into imex_fs values(538,1,'hw_product','catalog_entry','product_id','product_id','','hw_product');
insert into imex_fs values(539,0,'hw_product','product_to_program','product_id','product_id','','hw_product');
insert into imex_fs values(540,1,'application_program','product_to_program','program_id','program_id','','hw_product');
insert into imex_fs values(541,1,'application_program','product_to_program','program_id','pei_program_id','','hw_product');
insert into imex_fs values(542,0,'product_to_program','product_to_program_to_mt','prod2prog_id','prod2prog_id','','hw_product');
insert into imex_fs values(543,1,'medium_type','product_to_program_to_mt','medium_type_number','medium_type_number','','hw_product');
insert into imex_fs values(544,0,'application_program','text_attribute','program_id','entity_id','','hw_product');
insert into imex_fs values(545,0,'application_program','help_file','program_id','entity_id','','hw_product');
insert into imex_fs values(546,0,'application_program','program_description','program_id','program_id','','hw_product');
insert into imex_fs values(547,1,'ete_language','program_description','language_id','language_id','','hw_product');
insert into imex_fs values(548,0,'application_program','s19_block','program_id','program_id','','hw_product');
insert into imex_fs values(549,0,'s19_block','s19_block_paragraph','block_id','block_id','','hw_product');
insert into imex_fs values(550,0,'application_program','communication_object','program_id','program_id','','hw_product');
insert into imex_fs values(551,0,'application_program','address_fixup','program_id','program_id','','hw_product');
insert into imex_fs values(552,0,'application_program','parameter_type','program_id','program_id','','hw_product');
insert into imex_fs values(553,0,'application_program','parameter','program_id','program_id','par_parameter_id is null','hw_product');
insert into imex_fs values(554,0,'parameter_type','parameter','parameter_type_id','parameter_type_id','par_parameter_id is null','hw_product');
insert into imex_fs values(555,0,'parameter_type','parameter_list_of_values','parameter_type_id','parameter_type_id','','hw_product');
insert into imex_fs values(556,0,'parameter_list_of_values','text_attribute','parameter_value_id','entity_id','','hw_product');
insert into imex_fs values(557,0,'parameter','parameter','parameter_id','par_parameter_id','','hw_product');
insert into imex_fs values(558,0,'parameter','communication_object','parameter_id','parameter_id','','hw_product');
insert into imex_fs values(559,0,'parameter','symbol_condition','parameter_id','parameter_id','','hw_product');
insert into imex_fs values(560,0,'parameter','text_attribute','parameter_id','entity_id','','hw_product');
insert into imex_fs values(561,1,'parameter_atomic_type','parameter_type','atomic_type_number','atomic_type_number','','hw_product');
insert into imex_fs values(562,0,'communication_object','text_attribute','object_id','entity_id','','hw_product');
insert into imex_fs values(563,1,'object_type','communication_object','object_type_code','object_type','','hw_product');
insert into imex_fs values(564,1,'object_priority','communication_object','object_priority_code','object_priority','','hw_product');
insert into imex_fs values(565,1,'bcu_type','hw_product','bcu_type_number','bcu_type_number','','product_to_program');
insert into imex_fs values(566,1,'manufacturer','hw_product','manufacturer_id','manufacturer_id','','product_to_program');
insert into imex_fs values(567,1,'manufacturer','hw_product','manufacturer_id','original_manufacturer_id','','product_to_program');
insert into imex_fs values(568,1,'manufacturer','application_program','manufacturer_id','manufacturer_id','','product_to_program');
insert into imex_fs values(569,1,'manufacturer','application_program','manufacturer_id','original_manufacturer_id','','product_to_program');
insert into imex_fs values(570,1,'manufacturer','catalog_entry','manufacturer_id','manufacturer_id','','product_to_program');
insert into imex_fs values(571,1,'mask','application_program','mask_id','mask_id','','product_to_program');
insert into imex_fs values(572,1,'assembler','application_program','assembler_id','assembler_id','','product_to_program');
insert into imex_fs values(573,1,'symbol','application_program','symbol_id','symbol_id','','product_to_program');
insert into imex_fs values(574,1,'symbol','symbol_condition','symbol_id','symbol_id','','product_to_program');
insert into imex_fs values(575,1,'symbol','catalog_entry','symbol_id','symbol_id','','product_to_program');
insert into imex_fs values(576,1,'symbol','hw_product','symbol_id','symbol_id','','product_to_program');
insert into imex_fs values(577,1,'ete_language','text_attribute','language_id','language_id','','product_to_program');
insert into imex_fs values(578,1,'ete_language','help_file','language_id','language_id','','product_to_program');
insert into imex_fs values(579,0,'mask','mask_entry','mask_id','mask_id','','product_to_program');
insert into imex_fs values(580,0,'mask','mask_system_pointer','mask_id','mask_id','','product_to_program');
insert into imex_fs values(581,0,'mask','mask_compatibility','mask_id','new_mask_id','','product_to_program');
insert into imex_fs values(582,1,'mask','mask_compatibility','mask_id','old_mask_id','','product_to_program');
insert into imex_fs values(583,0,'mask','device_info','mask_id','mask_id','dev_device_info_id IS NULL','product_to_program');
insert into imex_fs values(584,0,'device_info','text_attribute','device_info_id','entity_id',NULL,'product_to_program');
insert into imex_fs values(585,0,'device_info','device_info','device_info_id','dev_device_info_id','','product_to_program');
insert into imex_fs values(586,0,'device_info','device_info_value','device_info_id','device_info_id','','product_to_program');
insert into imex_fs values(587,0,'device_info_value','text_attribute','device_info_value_id','entity_id',NULL,'product_to_program');
insert into imex_fs values(588,0,'catalog_entry','product_description','catalog_entry_id','catalog_entry_id','','product_to_program');
insert into imex_fs values(589,1,'ete_language','product_description','language_id','language_id','','product_to_program');
insert into imex_fs values(590,0,'catalog_entry','text_attribute','catalog_entry_id','entity_id','','product_to_program');
insert into imex_fs values(591,0,'catalog_entry','help_file','catalog_entry_id','entity_id','','product_to_program');
insert into imex_fs values(592,0,'catalog_entry','text_to_catent','catalog_entry_id','catalog_entry_id','','product_to_program');
insert into imex_fs values(593,1,'specification_text','text_to_catent','spec_text_id','spec_text_id','','product_to_program');
insert into imex_fs values(594,0,'hw_product','catalog_entry','product_id','product_id','entry_status_code > 4','product_to_program');
insert into imex_fs values(595,1,'hw_product','catalog_entry','product_id','product_id','','product_to_program');
insert into imex_fs values(596,1,'hw_product','product_to_program','product_id','product_id','','product_to_program');
insert into imex_fs values(597,1,'application_program','product_to_program','program_id','program_id','','product_to_program');
insert into imex_fs values(598,1,'application_program','product_to_program','program_id','pei_program_id','','product_to_program');
insert into imex_fs values(599,0,'product_to_program','product_to_program_to_mt','prod2prog_id','prod2prog_id','','product_to_program');
insert into imex_fs values(600,1,'medium_type','product_to_program_to_mt','medium_type_number','medium_type_number','','product_to_program');
insert into imex_fs values(601,0,'application_program','help_file','program_id','entity_id','','product_to_program');
insert into imex_fs values(602,0,'application_program','program_description','program_id','program_id','','product_to_program');
insert into imex_fs values(603,1,'ete_language','program_description','language_id','language_id','','product_to_program');
insert into imex_fs values(604,0,'application_program','s19_block','program_id','program_id','','product_to_program');
insert into imex_fs values(605,0,'s19_block','s19_block_paragraph','block_id','block_id','','product_to_program');
insert into imex_fs values(606,0,'application_program','communication_object','program_id','program_id','','product_to_program');
insert into imex_fs values(607,0,'application_program','address_fixup','program_id','program_id','','product_to_program');
insert into imex_fs values(608,0,'application_program','text_attribute','program_id','entity_id','','product_to_program');
insert into imex_fs values(609,0,'application_program','parameter_type','program_id','program_id','','product_to_program');
insert into imex_fs values(610,0,'application_program','parameter','program_id','program_id','par_parameter_id is null','product_to_program');
insert into imex_fs values(611,0,'parameter_type','parameter','parameter_type_id','parameter_type_id','par_parameter_id is null','product_to_program');
insert into imex_fs values(612,0,'parameter_type','parameter_list_of_values','parameter_type_id','parameter_type_id','','product_to_program');
insert into imex_fs values(613,0,'parameter_list_of_values','text_attribute','parameter_value_id','entity_id','','product_to_program');
insert into imex_fs values(614,0,'parameter','parameter','parameter_id','par_parameter_id','','product_to_program');
insert into imex_fs values(615,0,'parameter','communication_object','parameter_id','parameter_id','','product_to_program');
insert into imex_fs values(616,0,'parameter','symbol_condition','parameter_id','parameter_id','','product_to_program');
insert into imex_fs values(617,0,'parameter','text_attribute','parameter_id','entity_id','','product_to_program');
insert into imex_fs values(618,1,'parameter_atomic_type','parameter_type','atomic_type_number','atomic_type_number','','product_to_program');
insert into imex_fs values(619,0,'communication_object','text_attribute','object_id','entity_id','','product_to_program');
insert into imex_fs values(620,1,'object_type','communication_object','object_type_code','object_type','','product_to_program');
insert into imex_fs values(621,1,'object_priority','communication_object','object_priority_code','object_priority','','product_to_program');
insert into imex_fs values(622,1,'manufacturer','functional_entity','manufacturer_id','manufacturer_id','fun_functional_entity_id is null','product_to_program');
insert into imex_fs values(623,1,'functional_entity','functional_entity','functional_entity_id','fun_functional_entity_id','','product_to_program');
insert into imex_fs values(624,1,'functional_entity','virtual_device','functional_entity_id','functional_entity_id','','product_to_program');
insert into imex_fs values(625,1,'functional_entity','virtual_device','functional_entity_id','product_type_id','','product_to_program');
insert into imex_fs values(626,0,'functional_entity','text_attribute','functional_entity_id','entity_id','','product_to_program');
insert into imex_fs values(627,0,'functional_entity','help_file','functional_entity_id','entity_id','','product_to_program');
insert into imex_fs values(628,0,'functional_entity','maingroup','functional_entity_id','project_id','','product_to_program');
insert into imex_fs values(629,0,'maingroup','text_attribute','maingroup_id','entity_id','','product_to_program');
insert into imex_fs values(630,0,'maingroup','middlegroup','maingroup_id','maingroup_id','','product_to_program');
insert into imex_fs values(631,0,'middlegroup','subgroup','middlegroup_id','middlegroup_id','','product_to_program');
insert into imex_fs values(632,0,'middlegroup','text_attribute','middlegroup_id','entity_id','','product_to_program');
insert into imex_fs values(633,0,'subgroup','subgroup_to_object','subgroup_id','subgroup_id','','product_to_program');
insert into imex_fs values(634,0,'subgroup','text_attribute','subgroup_id','entity_id','','product_to_program');
insert into imex_fs values(635,0,'virtual_device','text_attribute','virtual_device_id','entity_id','','product_to_program');
insert into imex_fs values(636,0,'virtual_device','help_file','virtual_device_id','entity_id','','product_to_program');
insert into imex_fs values(637,0,'virtual_device','device_parameter','virtual_device_id','device_id','','product_to_program');
insert into imex_fs values(638,0,'virtual_device','device_object','virtual_device_id','device_id','','product_to_program');
insert into imex_fs values(639,0,'device_object','subgroup_to_object','device_object_id','device_object_id','','product_to_program');
insert into imex_fs values(640,0,'catalog_entry','virtual_device','catalog_entry_id','catalog_entry_id','','product_to_program');
insert into imex_fs values(641,1,'application_program','virtual_device','program_id','program_id','','product_to_program');
insert into imex_fs values(642,1,'application_program','virtual_device','program_id','pei_program_id','','product_to_program');
insert into imex_fs values(643,1,'symbol','virtual_device','symbol_id','symbol_id','','product_to_program');
insert into imex_fs values(644,0,'mask','mask_entry','mask_id','mask_id','','virtual_device');
insert into imex_fs values(645,0,'mask','mask_system_pointer','mask_id','mask_id','','virtual_device');
insert into imex_fs values(646,0,'mask','mask_compatibility','mask_id','new_mask_id','','virtual_device');
insert into imex_fs values(647,1,'mask','mask_compatibility','mask_id','old_mask_id','','virtual_device');
insert into imex_fs values(648,0,'mask','device_info','mask_id','mask_id','dev_device_info_id IS NULL','virtual_device');
insert into imex_fs values(649,0,'device_info','text_attribute','device_info_id','entity_id',NULL,'virtual_device');
insert into imex_fs values(650,0,'device_info','device_info','device_info_id','dev_device_info_id','','virtual_device');
insert into imex_fs values(651,0,'device_info','device_info_value','device_info_id','device_info_id','','virtual_device');
insert into imex_fs values(652,0,'device_info_value','text_attribute','device_info_value_id','entity_id',NULL,'virtual_device');
insert into imex_fs values(653,0,'catalog_entry','product_description','catalog_entry_id','catalog_entry_id','','virtual_device');
insert into imex_fs values(654,1,'ete_language','product_description','language_id','language_id','','virtual_device');
insert into imex_fs values(655,0,'catalog_entry','accessory','catalog_entry_id','cat_catalog_entry_id','','virtual_device');
insert into imex_fs values(656,1,'catalog_entry','accessory','catalog_entry_id','catalog_entry_id','','virtual_device');
insert into imex_fs values(657,0,'catalog_entry','text_attribute','catalog_entry_id','entity_id','','virtual_device');
insert into imex_fs values(658,0,'catalog_entry','help_file','catalog_entry_id','entity_id','','virtual_device');
insert into imex_fs values(659,0,'catalog_entry','text_to_catent','catalog_entry_id','catalog_entry_id','','virtual_device');
insert into imex_fs values(660,1,'specification_text','text_to_catent','spec_text_id','spec_text_id','','virtual_device');
insert into imex_fs values(661,0,'hw_product','product_to_program','product_id','product_id','','virtual_device');
insert into imex_fs values(662,1,'application_program','product_to_program','program_id','program_id','','virtual_device');
insert into imex_fs values(663,1,'application_program','product_to_program','program_id','pei_program_id','','virtual_device');
insert into imex_fs values(664,0,'product_to_program','product_to_program_to_mt','prod2prog_id','prod2prog_id','','virtual_device');
insert into imex_fs values(665,1,'medium_type','product_to_program_to_mt','medium_type_number','medium_type_number','','virtual_device');
insert into imex_fs values(666,0,'application_program','text_attribute','program_id','entity_id','','virtual_device');
insert into imex_fs values(667,0,'application_program','help_file','program_id','entity_id','','virtual_device');
insert into imex_fs values(668,0,'application_program','program_description','program_id','program_id','','virtual_device');
insert into imex_fs values(669,1,'ete_language','program_description','language_id','language_id','','virtual_device');
insert into imex_fs values(670,0,'application_program','s19_block','program_id','program_id','','virtual_device');
insert into imex_fs values(671,0,'s19_block','s19_block_paragraph','block_id','block_id','','virtual_device');
insert into imex_fs values(672,0,'application_program','communication_object','program_id','program_id','','virtual_device');
insert into imex_fs values(673,0,'application_program','address_fixup','program_id','program_id','','virtual_device');
insert into imex_fs values(674,0,'application_program','parameter_type','program_id','program_id','','virtual_device');
insert into imex_fs values(675,0,'application_program','parameter','program_id','program_id','par_parameter_id is null','virtual_device');
insert into imex_fs values(676,0,'parameter_type','parameter','parameter_type_id','parameter_type_id','par_parameter_id is null','virtual_device');
insert into imex_fs values(677,0,'parameter_type','parameter_list_of_values','parameter_type_id','parameter_type_id','','virtual_device');
insert into imex_fs values(678,0,'parameter_list_of_values','text_attribute','parameter_value_id','entity_id','','virtual_device');
insert into imex_fs values(679,0,'parameter','parameter','parameter_id','par_parameter_id','','virtual_device');
insert into imex_fs values(680,0,'parameter','communication_object','parameter_id','parameter_id','','virtual_device');
insert into imex_fs values(681,0,'parameter','symbol_condition','parameter_id','parameter_id','','virtual_device');
insert into imex_fs values(682,0,'parameter','text_attribute','parameter_id','entity_id','','virtual_device');
insert into imex_fs values(683,1,'parameter_atomic_type','parameter_type','atomic_type_number','atomic_type_number','','virtual_device');
insert into imex_fs values(684,0,'communication_object','text_attribute','object_id','entity_id','','virtual_device');
insert into imex_fs values(685,1,'object_type','communication_object','object_type_code','object_type','','virtual_device');
insert into imex_fs values(686,1,'object_priority','communication_object','object_priority_code','object_priority','','virtual_device');
insert into imex_fs values(687,1,'manufacturer','functional_entity','manufacturer_id','manufacturer_id','fun_functional_entity_id is null','virtual_device');
insert into imex_fs values(688,1,'functional_entity','functional_entity','functional_entity_id','fun_functional_entity_id','','virtual_device');
insert into imex_fs values(689,1,'functional_entity','virtual_device','functional_entity_id','functional_entity_id','','virtual_device');
insert into imex_fs values(690,1,'functional_entity','virtual_device','functional_entity_id','product_type_id','','virtual_device');
insert into imex_fs values(691,1,'ete_language','text_attribute','language_id','language_id','','virtual_device');
insert into imex_fs values(692,1,'ete_language','help_file','language_id','language_id','','virtual_device');
insert into imex_fs values(693,0,'functional_entity','text_attribute','functional_entity_id','entity_id','','virtual_device');
insert into imex_fs values(694,0,'functional_entity','help_file','functional_entity_id','entity_id','','virtual_device');
insert into imex_fs values(695,0,'functional_entity','maingroup','functional_entity_id','project_id','','virtual_device');
insert into imex_fs values(696,0,'maingroup','text_attribute','maingroup_id','entity_id','','virtual_device');
insert into imex_fs values(697,0,'maingroup','middlegroup','maingroup_id','maingroup_id','','virtual_device');
insert into imex_fs values(698,0,'middlegroup','subgroup','middlegroup_id','middlegroup_id','','virtual_device');
insert into imex_fs values(699,0,'middlegroup','text_attribute','middlegroup_id','entity_id','','virtual_device');
insert into imex_fs values(700,0,'subgroup','subgroup_to_object','subgroup_id','subgroup_id','','virtual_device');
insert into imex_fs values(701,0,'subgroup','text_attribute','subgroup_id','entity_id','','virtual_device');
insert into imex_fs values(702,0,'virtual_device','text_attribute','virtual_device_id','entity_id','','virtual_device');
insert into imex_fs values(703,0,'virtual_device','help_file','virtual_device_id','entity_id','','virtual_device');
insert into imex_fs values(704,0,'virtual_device','device_parameter','virtual_device_id','device_id','','virtual_device');
insert into imex_fs values(705,0,'virtual_device','device_object','virtual_device_id','device_id','','virtual_device');
insert into imex_fs values(706,0,'device_object','subgroup_to_object','device_object_id','device_object_id','','virtual_device');
insert into imex_fs values(707,0,'virtual_device','device_accessory','virtual_device_id','device_id','','virtual_device');
insert into imex_fs values(708,1,'catalog_entry','device_accessory','catalog_entry_id','catalog_entry_id','','virtual_device');
insert into imex_fs values(709,1,'catalog_entry','virtual_device','catalog_entry_id','catalog_entry_id','','virtual_device');
insert into imex_fs values(710,1,'hw_product','catalog_entry','product_id','product_id','','virtual_device');
insert into imex_fs values(711,1,'bcu_type','hw_product','bcu_type_number','bcu_type_number','','virtual_device');
insert into imex_fs values(712,1,'application_program','virtual_device','program_id','program_id','','virtual_device');
insert into imex_fs values(713,1,'application_program','virtual_device','program_id','pei_program_id','','virtual_device');
insert into imex_fs values(714,1,'manufacturer','hw_product','manufacturer_id','manufacturer_id','','virtual_device');
insert into imex_fs values(715,1,'manufacturer','hw_product','manufacturer_id','original_manufacturer_id','','virtual_device');
insert into imex_fs values(716,1,'manufacturer','application_program','manufacturer_id','manufacturer_id','','virtual_device');
insert into imex_fs values(717,1,'manufacturer','application_program','manufacturer_id','original_manufacturer_id','','virtual_device');
insert into imex_fs values(718,1,'manufacturer','catalog_entry','manufacturer_id','manufacturer_id','','virtual_device');
insert into imex_fs values(719,1,'mask','application_program','mask_id','mask_id','','virtual_device');
insert into imex_fs values(720,1,'assembler','application_program','assembler_id','assembler_id','','virtual_device');
insert into imex_fs values(721,1,'symbol','application_program','symbol_id','symbol_id','','virtual_device');
insert into imex_fs values(722,1,'symbol','symbol_condition','symbol_id','symbol_id','','virtual_device');
insert into imex_fs values(723,1,'symbol','catalog_entry','symbol_id','symbol_id','','virtual_device');
insert into imex_fs values(724,1,'symbol','hw_product','symbol_id','symbol_id','','virtual_device');
insert into imex_fs values(725,1,'symbol','virtual_device','symbol_id','symbol_id','','virtual_device');
insert into imex_fs values(726,0,'mask','mask_entry','mask_id','mask_id','','mask');
insert into imex_fs values(727,0,'mask','mask_system_pointer','mask_id','mask_id','','mask');
insert into imex_fs values(728,0,'mask','mask_compatibility','mask_id','new_mask_id','','mask');
insert into imex_fs values(729,1,'mask','mask_compatibility','mask_id','old_mask_id','','mask');
insert into imex_fs values(730,0,'mask','device_info','mask_id','mask_id','dev_device_info_id IS NULL','mask');
insert into imex_fs values(731,0,'device_info','text_attribute','device_info_id','entity_id',NULL,'mask');
insert into imex_fs values(732,0,'device_info','device_info','device_info_id','dev_device_info_id','','mask');
insert into imex_fs values(733,0,'device_info','device_info_value','device_info_id','device_info_id','','mask');
insert into imex_fs values(734,0,'device_info_value','text_attribute','device_info_value_id','entity_id',NULL,'mask');
insert into imex_fs values(735,1,'ete_language','text_attribute','language_id','language_id','','mask');

commit;

#
#  IMEX-KEYS einfügen.
#
insert into imex_keys values(90,'manufacturer','manufacturer_id');
insert into imex_keys values(91,'manufacturer_compatibility','manufacturer_id');
insert into imex_keys values(92,'manufacturer_compatibility','man_manufacturer_id');
insert into imex_keys values(93,'component_type','component_type_number');
insert into imex_keys values(94,'bcu_type','bcu_type_number');
insert into imex_keys values(95,'pei_type','pei_type_number');
insert into imex_keys values(96,'assembler','assembler_name');
insert into imex_keys values(97,'object_type','object_type_code');
insert into imex_keys values(98,'object_priority','object_priority_code');
insert into imex_keys values(99,'currency','currency_name');
insert into imex_keys values(100,'quantity_unit','quantity_unit_name');
insert into imex_keys values(101,'symbol','symbol_id');
insert into imex_keys values(102,'person','person_name');
insert into imex_keys values(103,'address_format','address_format_name');
insert into imex_keys values(104,'medium_type','medium_type_number');
insert into imex_keys values(105,'product_to_program','product_id');
insert into imex_keys values(106,'product_to_program','program_id');
insert into imex_keys values(107,'product_to_program','pei_program_id');
insert into imex_keys values(108,'product_to_program_to_mt','prod2prog_id');
insert into imex_keys values(109,'product_to_program_to_mt','medium_type_number');
insert into imex_keys values(110,'hw_product','manufacturer_id');
insert into imex_keys values(111,'hw_product','product_serial_number');
insert into imex_keys values(112,'hw_product','product_version_number');
insert into imex_keys values(113,'mask','mask_version');
insert into imex_keys values(114,'mask_entry','mask_id');
insert into imex_keys values(115,'mask_entry','mask_entry_name');
insert into imex_keys values(116,'mask_system_pointer','mask_id');
insert into imex_keys values(117,'mask_system_pointer','system_pointer_name');
insert into imex_keys values(118,'device_info','mask_id');
insert into imex_keys values(119,'device_info','device_info_name');
insert into imex_keys values(120,'device_info_value','device_info_id');
insert into imex_keys values(121,'device_info_value','minimum_value');
insert into imex_keys values(122,'device_info_value','maximum_value');
insert into imex_keys values(123,'mask_compatibility','old_mask_id');
insert into imex_keys values(124,'mask_compatibility','new_mask_id');
insert into imex_keys values(125,'application_program','manufacturer_id');
insert into imex_keys values(126,'application_program','device_type');
insert into imex_keys values(127,'application_program','program_version');
insert into imex_keys values(128,'address_fixup','program_id');
insert into imex_keys values(129,'address_fixup','fixup_address');
insert into imex_keys values(130,'program_description','program_id');
insert into imex_keys values(131,'program_description','display_order');
insert into imex_keys values(132,'program_description','language_id');
insert into imex_keys values(133,'s19_block','program_id');
insert into imex_keys values(134,'s19_block','block_number');
insert into imex_keys values(135,'s19_block_paragraph','block_id');
insert into imex_keys values(136,'s19_block_paragraph','pt_column_id');
insert into imex_keys values(137,'catalog_entry','manufacturer_id');
insert into imex_keys values(138,'catalog_entry','order_number');
insert into imex_keys values(139,'accessory','catalog_entry_id');
insert into imex_keys values(140,'accessory','cat_catalog_entry_id');
insert into imex_keys values(141,'product_description','catalog_entry_id');
insert into imex_keys values(142,'product_description','display_order');
insert into imex_keys values(143,'product_description','language_id');
insert into imex_keys values(144,'specification_text','manufacturer_id');
insert into imex_keys values(145,'specification_text','spec_text_filename');
insert into imex_keys values(146,'text_to_catent','spec_text');
insert into imex_keys values(147,'text_to_catent','catalog_entry_id');
insert into imex_keys values(148,'parameter_atomic_type','atomic_type_number');
insert into imex_keys values(149,'parameter_type','program_id');
insert into imex_keys values(150,'parameter_type','parameter_type_name');
insert into imex_keys values(151,'parameter_list_of_values','parameter_type_id');
insert into imex_keys values(152,'parameter_list_of_values','real_value');
insert into imex_keys values(153,'parameter','program_id');
insert into imex_keys values(154,'parameter','parameter_number');
insert into imex_keys values(155,'communication_object','program_id');
insert into imex_keys values(156,'communication_object','object_unique_number');
insert into imex_keys values(157,'parameter_list_of_values','parameter_type_id');
insert into imex_keys values(158,'parameter_list_of_values','real_value');
insert into imex_keys values(159,'symbol_condition','parameter_id');
insert into imex_keys values(160,'symbol_condition','parent_parm_value');
insert into imex_keys values(161,'functional_entity','manufacturer_id');
insert into imex_keys values(162,'functional_entity','fun_functional_entity_id');
insert into imex_keys values(163,'functional_entity','functional_entity_numb');
insert into imex_keys values(164,'maingroup','project_id');
insert into imex_keys values(165,'maingroup','maingroup_address');
insert into imex_keys values(166,'middlegroup','maingroup_id');
insert into imex_keys values(167,'middlegroup','middlegroup_address');
insert into imex_keys values(168,'subgroup','middlegroup_id');
insert into imex_keys values(169,'subgroup','subgroup_address');
insert into imex_keys values(170,'virtual_device','catalog_entry_id');
insert into imex_keys values(171,'virtual_device','program_id');
insert into imex_keys values(172,'virtual_device','pei_program_id');
insert into imex_keys values(173,'virtual_device','virtual_device_number');
insert into imex_keys values(174,'device_parameter','device_id');
insert into imex_keys values(175,'device_parameter','parameter_id');
insert into imex_keys values(176,'device_object','device_id');
insert into imex_keys values(177,'device_object','object_id');
insert into imex_keys values(178,'device_accessory','device_id');
insert into imex_keys values(179,'device_accessory','catalog_entry_id');
insert into imex_keys values(180,'ete_language','language_id');
insert into imex_keys values(181,'text_attribute','language_id');
insert into imex_keys values(182,'text_attribute','column_id');
insert into imex_keys values(183,'text_attribute','entity_id');
insert into imex_keys values(184,'help_file','language_id');
insert into imex_keys values(185,'help_file','entity_id');

commit;

#
#  IMEX-OMIT einfügen
#
insert into imex_omit values(1,2,7,'medium_type','');
insert into imex_omit values(2,2,7,'product_to_program_to_mt','');
insert into imex_omit values(3,2,7,'line','medium_type_number');
insert into imex_omit values(4,2,7,'line','system_id');
insert into imex_omit values(5,2,7,'line','system_id_scan');
insert into imex_omit values(6,2,7,'device_programming','repeater_flag');
insert into imex_omit values(7,2,7,'system_id_lookup','');
insert into imex_omit values(8,2,7,'application_program','program_style');
insert into imex_omit values(9,2,7,'s19_block_paragraph','');
insert into imex_omit values(10,2,7,'project','system_id');
insert into imex_omit values(11,2,7,'project','system_id_scan');
insert into imex_omit values(12,2,7,'project','repeater_mode');
insert into imex_omit values(13,2,7,'area','system_id');
insert into imex_omit values(14,2,7,'area','system_id_scan');
insert into imex_omit values(15,2,7,'device_info','');
insert into imex_omit values(16,2,7,'device_info_value','');
insert into imex_omit values(17,2,7,'area','repeater_mode');
insert into imex_omit values(18,2,7,'line','repeater_mode');
insert into imex_omit values(19,2,7,'polling_group','');
insert into imex_omit values(20,2,7,'device_to_polling_group','');
insert into imex_omit values(21,2,7,'device','polling_master_text');
insert into imex_omit values(22,2,7,'device','polling_slave_text');
insert into imex_omit values(23,2,7,'application_program','is_polling_master');
insert into imex_omit values(24,2,7,'application_program','number_of_polling_groups');
insert into imex_omit values(25,2,7,'s19_block','block_mask');
insert into imex_omit values(26,2,7,'sysid_scan',NULL);
insert into imex_omit values(27,2,7,'device','is_dummy');
insert into imex_omit values(28,2,7,'medium_type','medium_type_short_name');
insert into imex_omit values(29,2,7,'virtual_device','medium_types');
insert into imex_omit values(30,2,7,'parameter','patch_always');

commit;

#
#  IMEX-TABLE-ORDER einfügen.
#
insert into imex_table_order values('communication_object',1,'device_object');
insert into imex_table_order values('parameter',1,'device_parameter');

commit;

#
#  IMEX-WHERE-CLAUSE einfügen.
#
insert into imex_where_clause values(7,'project','device','where project_id = %ld');
insert into imex_where_clause values(8,'project','device_object','where device_id in ( select device_id from delta.device where project_id = %ld )');
insert into imex_where_clause values(9,'project','device_parameter','where device_id in ( select device_id from delta.device where project_id = %ld )');
insert into imex_where_clause values(10,'project','device_accessory','where device_id in ( select device_id from delta.device where project_id = %ld )');
insert into imex_where_clause values(11,'project','device_programming','where device_id in ( select device_id from delta.device where project_id = %ld )');
insert into imex_where_clause values(12,'project','subgroup_to_object','where subgroup_id in ( select subgroup_id from delta.subgroup key join delta.middlegroup key join delta.maingroup where project_id = %ld )');

commit;



#
#  aus der ETS 1.3 übernehmen:
#
# channel_list
# channel_list_to_channel
# duty_cycle_ondition
# eagle_info
# eib_data_subtype
# eib_data_type
# ets_database_version
# mask_to_mask_feature
# medium_channel
# schema_columns
# channel_list
# channel_list_to_channel
# duty_cycle_ondition
# eagle_info
# eib_data_subtype
# eib_data_type
# ets_database_version
# mask_to_mask_feature
# medium_channel
# schema_columns
# schema_columns_h
# schema_provider_types
# schema_tablesschema_columns_h
# schema_provider_types
# schema_tables