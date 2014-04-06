<?php

namespace Map;

use \Msg;
use \MsgQuery;
use Propel\Runtime\Propel;
use Propel\Runtime\ActiveQuery\Criteria;
use Propel\Runtime\ActiveQuery\InstancePoolTrait;
use Propel\Runtime\Connection\ConnectionInterface;
use Propel\Runtime\DataFetcher\DataFetcherInterface;
use Propel\Runtime\Exception\PropelException;
use Propel\Runtime\Map\RelationMap;
use Propel\Runtime\Map\TableMap;
use Propel\Runtime\Map\TableMapTrait;


/**
 * This class defines the structure of the 'msg' table.
 *
 *
 *
 * This map class is used by Propel to do runtime db structure discovery.
 * For example, the createSelectSql() method checks the type of a given column used in an
 * ORDER BY clause to know whether it needs to apply SQL to make the ORDER BY case-insensitive
 * (i.e. if it's a text column type).
 *
 */
class MsgTableMap extends TableMap
{
    use InstancePoolTrait;
    use TableMapTrait;

    /**
     * The (dot-path) name of this class
     */
    const CLASS_NAME = '.Map.MsgTableMap';

    /**
     * The default database name for this class
     */
    const DATABASE_NAME = 'qaulhub';

    /**
     * The table name for this class
     */
    const TABLE_NAME = 'msg';

    /**
     * The related Propel class for this table
     */
    const OM_CLASS = '\\Msg';

    /**
     * A class that can be returned by this tableMap
     */
    const CLASS_DEFAULT = 'Msg';

    /**
     * The total number of columns
     */
    const NUM_COLUMNS = 10;

    /**
     * The number of lazy-loaded columns
     */
    const NUM_LAZY_LOAD_COLUMNS = 0;

    /**
     * The number of columns to hydrate (NUM_COLUMNS - NUM_LAZY_LOAD_COLUMNS)
     */
    const NUM_HYDRATE_COLUMNS = 10;

    /**
     * the column name for the ID field
     */
    const COL_ID = 'msg.ID';

    /**
     * the column name for the TYPE field
     */
    const COL_TYPE = 'msg.TYPE';

    /**
     * the column name for the NAME field
     */
    const COL_NAME = 'msg.NAME';

    /**
     * the column name for the MSG field
     */
    const COL_MSG = 'msg.MSG';

    /**
     * the column name for the IP field
     */
    const COL_IP = 'msg.IP';

    /**
     * the column name for the TIME field
     */
    const COL_TIME = 'msg.TIME';

    /**
     * the column name for the STATUS field
     */
    const COL_STATUS = 'msg.STATUS';

    /**
     * the column name for the TWITTERID field
     */
    const COL_TWITTERID = 'msg.TWITTERID';

    /**
     * the column name for the CREATED_AT field
     */
    const COL_CREATED_AT = 'msg.CREATED_AT';

    /**
     * the column name for the UPDATED_AT field
     */
    const COL_UPDATED_AT = 'msg.UPDATED_AT';

    /**
     * The default string format for model objects of the related table
     */
    const DEFAULT_STRING_FORMAT = 'YAML';

    /**
     * holds an array of fieldnames
     *
     * first dimension keys are the type constants
     * e.g. self::$fieldNames[self::TYPE_PHPNAME][0] = 'Id'
     */
    protected static $fieldNames = array (
        self::TYPE_PHPNAME       => array('Id', 'Type', 'Name', 'Msg', 'Ip', 'Time', 'Status', 'Twitterid', 'CreatedAt', 'UpdatedAt', ),
        self::TYPE_STUDLYPHPNAME => array('id', 'type', 'name', 'msg', 'ip', 'time', 'status', 'twitterid', 'createdAt', 'updatedAt', ),
        self::TYPE_COLNAME       => array(MsgTableMap::COL_ID, MsgTableMap::COL_TYPE, MsgTableMap::COL_NAME, MsgTableMap::COL_MSG, MsgTableMap::COL_IP, MsgTableMap::COL_TIME, MsgTableMap::COL_STATUS, MsgTableMap::COL_TWITTERID, MsgTableMap::COL_CREATED_AT, MsgTableMap::COL_UPDATED_AT, ),
        self::TYPE_RAW_COLNAME   => array('COL_ID', 'COL_TYPE', 'COL_NAME', 'COL_MSG', 'COL_IP', 'COL_TIME', 'COL_STATUS', 'COL_TWITTERID', 'COL_CREATED_AT', 'COL_UPDATED_AT', ),
        self::TYPE_FIELDNAME     => array('id', 'type', 'name', 'msg', 'ip', 'time', 'status', 'twitterid', 'created_at', 'updated_at', ),
        self::TYPE_NUM           => array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, )
    );

    /**
     * holds an array of keys for quick access to the fieldnames array
     *
     * first dimension keys are the type constants
     * e.g. self::$fieldKeys[self::TYPE_PHPNAME]['Id'] = 0
     */
    protected static $fieldKeys = array (
        self::TYPE_PHPNAME       => array('Id' => 0, 'Type' => 1, 'Name' => 2, 'Msg' => 3, 'Ip' => 4, 'Time' => 5, 'Status' => 6, 'Twitterid' => 7, 'CreatedAt' => 8, 'UpdatedAt' => 9, ),
        self::TYPE_STUDLYPHPNAME => array('id' => 0, 'type' => 1, 'name' => 2, 'msg' => 3, 'ip' => 4, 'time' => 5, 'status' => 6, 'twitterid' => 7, 'createdAt' => 8, 'updatedAt' => 9, ),
        self::TYPE_COLNAME       => array(MsgTableMap::COL_ID => 0, MsgTableMap::COL_TYPE => 1, MsgTableMap::COL_NAME => 2, MsgTableMap::COL_MSG => 3, MsgTableMap::COL_IP => 4, MsgTableMap::COL_TIME => 5, MsgTableMap::COL_STATUS => 6, MsgTableMap::COL_TWITTERID => 7, MsgTableMap::COL_CREATED_AT => 8, MsgTableMap::COL_UPDATED_AT => 9, ),
        self::TYPE_RAW_COLNAME   => array('COL_ID' => 0, 'COL_TYPE' => 1, 'COL_NAME' => 2, 'COL_MSG' => 3, 'COL_IP' => 4, 'COL_TIME' => 5, 'COL_STATUS' => 6, 'COL_TWITTERID' => 7, 'COL_CREATED_AT' => 8, 'COL_UPDATED_AT' => 9, ),
        self::TYPE_FIELDNAME     => array('id' => 0, 'type' => 1, 'name' => 2, 'msg' => 3, 'ip' => 4, 'time' => 5, 'status' => 6, 'twitterid' => 7, 'created_at' => 8, 'updated_at' => 9, ),
        self::TYPE_NUM           => array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, )
    );

    /**
     * Initialize the table attributes and columns
     * Relations are not initialized by this method since they are lazy loaded
     *
     * @return void
     * @throws PropelException
     */
    public function initialize()
    {
        // attributes
        $this->setName('msg');
        $this->setPhpName('Msg');
        $this->setClassName('\\Msg');
        $this->setPackage('');
        $this->setUseIdGenerator(true);
        // columns
        $this->addPrimaryKey('ID', 'Id', 'INTEGER', true, null, null);
        $this->addColumn('TYPE', 'Type', 'INTEGER', true, null, null);
        $this->addColumn('NAME', 'Name', 'VARCHAR', true, 255, null);
        $this->addColumn('MSG', 'Msg', 'VARCHAR', true, 255, null);
        $this->addColumn('IP', 'Ip', 'VARCHAR', true, 255, null);
        $this->addColumn('TIME', 'Time', 'VARCHAR', true, 255, null);
        $this->addColumn('STATUS', 'Status', 'INTEGER', true, null, null);
        $this->addColumn('TWITTERID', 'Twitterid', 'VARCHAR', false, 255, null);
        $this->addColumn('CREATED_AT', 'CreatedAt', 'TIMESTAMP', false, null, null);
        $this->addColumn('UPDATED_AT', 'UpdatedAt', 'TIMESTAMP', false, null, null);
    } // initialize()

    /**
     * Build the RelationMap objects for this table relationships
     */
    public function buildRelations()
    {
    } // buildRelations()

    /**
     *
     * Gets the list of behaviors registered for this table
     *
     * @return array Associative array (name => parameters) of behaviors
     */
    public function getBehaviors()
    {
        return array(
            'timestampable' => array('create_column' => 'created_at', 'update_column' => 'updated_at', ),
        );
    } // getBehaviors()

    /**
     * Retrieves a string version of the primary key from the DB resultset row that can be used to uniquely identify a row in this table.
     *
     * For tables with a single-column primary key, that simple pkey value will be returned.  For tables with
     * a multi-column primary key, a serialize()d version of the primary key will be returned.
     *
     * @param array  $row       resultset row.
     * @param int    $offset    The 0-based offset for reading from the resultset row.
     * @param string $indexType One of the class type constants TableMap::TYPE_PHPNAME, TableMap::TYPE_STUDLYPHPNAME
     *                           TableMap::TYPE_COLNAME, TableMap::TYPE_FIELDNAME, TableMap::TYPE_NUM
     *
     * @return string The primary key hash of the row
     */
    public static function getPrimaryKeyHashFromRow($row, $offset = 0, $indexType = TableMap::TYPE_NUM)
    {
        // If the PK cannot be derived from the row, return NULL.
        if ($row[TableMap::TYPE_NUM == $indexType ? 0 + $offset : static::translateFieldName('Id', TableMap::TYPE_PHPNAME, $indexType)] === null) {
            return null;
        }

        return (string) $row[TableMap::TYPE_NUM == $indexType ? 0 + $offset : static::translateFieldName('Id', TableMap::TYPE_PHPNAME, $indexType)];
    }

    /**
     * Retrieves the primary key from the DB resultset row
     * For tables with a single-column primary key, that simple pkey value will be returned.  For tables with
     * a multi-column primary key, an array of the primary key columns will be returned.
     *
     * @param array  $row       resultset row.
     * @param int    $offset    The 0-based offset for reading from the resultset row.
     * @param string $indexType One of the class type constants TableMap::TYPE_PHPNAME, TableMap::TYPE_STUDLYPHPNAME
     *                           TableMap::TYPE_COLNAME, TableMap::TYPE_FIELDNAME, TableMap::TYPE_NUM
     *
     * @return mixed The primary key of the row
     */
    public static function getPrimaryKeyFromRow($row, $offset = 0, $indexType = TableMap::TYPE_NUM)
    {
        return (int) $row[
            $indexType == TableMap::TYPE_NUM
                ? 0 + $offset
                : self::translateFieldName('Id', TableMap::TYPE_PHPNAME, $indexType)
        ];
    }

    /**
     * The class that the tableMap will make instances of.
     *
     * If $withPrefix is true, the returned path
     * uses a dot-path notation which is translated into a path
     * relative to a location on the PHP include_path.
     * (e.g. path.to.MyClass -> 'path/to/MyClass.php')
     *
     * @param boolean $withPrefix Whether or not to return the path with the class name
     * @return string path.to.ClassName
     */
    public static function getOMClass($withPrefix = true)
    {
        return $withPrefix ? MsgTableMap::CLASS_DEFAULT : MsgTableMap::OM_CLASS;
    }

    /**
     * Populates an object of the default type or an object that inherit from the default.
     *
     * @param array  $row       row returned by DataFetcher->fetch().
     * @param int    $offset    The 0-based offset for reading from the resultset row.
     * @param string $indexType The index type of $row. Mostly DataFetcher->getIndexType().
                                 One of the class type constants TableMap::TYPE_PHPNAME, TableMap::TYPE_STUDLYPHPNAME
     *                           TableMap::TYPE_COLNAME, TableMap::TYPE_FIELDNAME, TableMap::TYPE_NUM.
     *
     * @throws PropelException Any exceptions caught during processing will be
     *         rethrown wrapped into a PropelException.
     * @return array (Msg object, last column rank)
     */
    public static function populateObject($row, $offset = 0, $indexType = TableMap::TYPE_NUM)
    {
        $key = MsgTableMap::getPrimaryKeyHashFromRow($row, $offset, $indexType);
        if (null !== ($obj = MsgTableMap::getInstanceFromPool($key))) {
            // We no longer rehydrate the object, since this can cause data loss.
            // See http://www.propelorm.org/ticket/509
            // $obj->hydrate($row, $offset, true); // rehydrate
            $col = $offset + MsgTableMap::NUM_HYDRATE_COLUMNS;
        } else {
            $cls = MsgTableMap::OM_CLASS;
            /** @var Msg $obj */
            $obj = new $cls();
            $col = $obj->hydrate($row, $offset, false, $indexType);
            MsgTableMap::addInstanceToPool($obj, $key);
        }

        return array($obj, $col);
    }

    /**
     * The returned array will contain objects of the default type or
     * objects that inherit from the default.
     *
     * @param DataFetcherInterface $dataFetcher
     * @return array
     * @throws PropelException Any exceptions caught during processing will be
     *         rethrown wrapped into a PropelException.
     */
    public static function populateObjects(DataFetcherInterface $dataFetcher)
    {
        $results = array();

        // set the class once to avoid overhead in the loop
        $cls = static::getOMClass(false);
        // populate the object(s)
        while ($row = $dataFetcher->fetch()) {
            $key = MsgTableMap::getPrimaryKeyHashFromRow($row, 0, $dataFetcher->getIndexType());
            if (null !== ($obj = MsgTableMap::getInstanceFromPool($key))) {
                // We no longer rehydrate the object, since this can cause data loss.
                // See http://www.propelorm.org/ticket/509
                // $obj->hydrate($row, 0, true); // rehydrate
                $results[] = $obj;
            } else {
                /** @var Msg $obj */
                $obj = new $cls();
                $obj->hydrate($row);
                $results[] = $obj;
                MsgTableMap::addInstanceToPool($obj, $key);
            } // if key exists
        }

        return $results;
    }
    /**
     * Add all the columns needed to create a new object.
     *
     * Note: any columns that were marked with lazyLoad="true" in the
     * XML schema will not be added to the select list and only loaded
     * on demand.
     *
     * @param Criteria $criteria object containing the columns to add.
     * @param string   $alias    optional table alias
     * @throws PropelException Any exceptions caught during processing will be
     *         rethrown wrapped into a PropelException.
     */
    public static function addSelectColumns(Criteria $criteria, $alias = null)
    {
        if (null === $alias) {
            $criteria->addSelectColumn(MsgTableMap::COL_ID);
            $criteria->addSelectColumn(MsgTableMap::COL_TYPE);
            $criteria->addSelectColumn(MsgTableMap::COL_NAME);
            $criteria->addSelectColumn(MsgTableMap::COL_MSG);
            $criteria->addSelectColumn(MsgTableMap::COL_IP);
            $criteria->addSelectColumn(MsgTableMap::COL_TIME);
            $criteria->addSelectColumn(MsgTableMap::COL_STATUS);
            $criteria->addSelectColumn(MsgTableMap::COL_TWITTERID);
            $criteria->addSelectColumn(MsgTableMap::COL_CREATED_AT);
            $criteria->addSelectColumn(MsgTableMap::COL_UPDATED_AT);
        } else {
            $criteria->addSelectColumn($alias . '.ID');
            $criteria->addSelectColumn($alias . '.TYPE');
            $criteria->addSelectColumn($alias . '.NAME');
            $criteria->addSelectColumn($alias . '.MSG');
            $criteria->addSelectColumn($alias . '.IP');
            $criteria->addSelectColumn($alias . '.TIME');
            $criteria->addSelectColumn($alias . '.STATUS');
            $criteria->addSelectColumn($alias . '.TWITTERID');
            $criteria->addSelectColumn($alias . '.CREATED_AT');
            $criteria->addSelectColumn($alias . '.UPDATED_AT');
        }
    }

    /**
     * Returns the TableMap related to this object.
     * This method is not needed for general use but a specific application could have a need.
     * @return TableMap
     * @throws PropelException Any exceptions caught during processing will be
     *         rethrown wrapped into a PropelException.
     */
    public static function getTableMap()
    {
        return Propel::getServiceContainer()->getDatabaseMap(MsgTableMap::DATABASE_NAME)->getTable(MsgTableMap::TABLE_NAME);
    }

    /**
     * Add a TableMap instance to the database for this tableMap class.
     */
    public static function buildTableMap()
    {
        $dbMap = Propel::getServiceContainer()->getDatabaseMap(MsgTableMap::DATABASE_NAME);
        if (!$dbMap->hasTable(MsgTableMap::TABLE_NAME)) {
            $dbMap->addTableObject(new MsgTableMap());
        }
    }

    /**
     * Performs a DELETE on the database, given a Msg or Criteria object OR a primary key value.
     *
     * @param mixed               $values Criteria or Msg object or primary key or array of primary keys
     *              which is used to create the DELETE statement
     * @param ConnectionInterface $con the connection to use
     * @return int The number of affected rows (if supported by underlying database driver).  This includes CASCADE-related rows
     *                if supported by native driver or if emulated using Propel.
     * @throws PropelException Any exceptions caught during processing will be
     *         rethrown wrapped into a PropelException.
     */
     public static function doDelete($values, ConnectionInterface $con = null)
     {
        if (null === $con) {
            $con = Propel::getServiceContainer()->getWriteConnection(MsgTableMap::DATABASE_NAME);
        }

        if ($values instanceof Criteria) {
            // rename for clarity
            $criteria = $values;
        } elseif ($values instanceof \Msg) { // it's a model object
            // create criteria based on pk values
            $criteria = $values->buildPkeyCriteria();
        } else { // it's a primary key, or an array of pks
            $criteria = new Criteria(MsgTableMap::DATABASE_NAME);
            $criteria->add(MsgTableMap::COL_ID, (array) $values, Criteria::IN);
        }

        $query = MsgQuery::create()->mergeWith($criteria);

        if ($values instanceof Criteria) {
            MsgTableMap::clearInstancePool();
        } elseif (!is_object($values)) { // it's a primary key, or an array of pks
            foreach ((array) $values as $singleval) {
                MsgTableMap::removeInstanceFromPool($singleval);
            }
        }

        return $query->delete($con);
    }

    /**
     * Deletes all rows from the msg table.
     *
     * @param ConnectionInterface $con the connection to use
     * @return int The number of affected rows (if supported by underlying database driver).
     */
    public static function doDeleteAll(ConnectionInterface $con = null)
    {
        return MsgQuery::create()->doDeleteAll($con);
    }

    /**
     * Performs an INSERT on the database, given a Msg or Criteria object.
     *
     * @param mixed               $criteria Criteria or Msg object containing data that is used to create the INSERT statement.
     * @param ConnectionInterface $con the ConnectionInterface connection to use
     * @return mixed           The new primary key.
     * @throws PropelException Any exceptions caught during processing will be
     *         rethrown wrapped into a PropelException.
     */
    public static function doInsert($criteria, ConnectionInterface $con = null)
    {
        if (null === $con) {
            $con = Propel::getServiceContainer()->getWriteConnection(MsgTableMap::DATABASE_NAME);
        }

        if ($criteria instanceof Criteria) {
            $criteria = clone $criteria; // rename for clarity
        } else {
            $criteria = $criteria->buildCriteria(); // build Criteria from Msg object
        }

        if ($criteria->containsKey(MsgTableMap::COL_ID) && $criteria->keyContainsValue(MsgTableMap::COL_ID) ) {
            throw new PropelException('Cannot insert a value for auto-increment primary key ('.MsgTableMap::COL_ID.')');
        }


        // Set the correct dbName
        $query = MsgQuery::create()->mergeWith($criteria);

        // use transaction because $criteria could contain info
        // for more than one table (I guess, conceivably)
        return $con->transaction(function () use ($con, $query) {
            return $query->doInsert($con);
        });
    }

} // MsgTableMap
// This is the static code needed to register the TableMap for this table with the main Propel class.
//
MsgTableMap::buildTableMap();
