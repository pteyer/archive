package edu.ucla.cs.ndntweets.db;

import java.lang.reflect.ParameterizedType;
import java.sql.SQLException;
import java.util.List;

import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.ResultSetHandler;
import org.apache.commons.dbutils.handlers.BeanListHandler;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class GenericDaoImplOld<T> implements GenericDaoOld<T> {

    private static final Log log = LogFactory.getLog(GenericDaoImplOld.class);
    
    private Class<T> persistClass = null;
    private QueryRunner qrunner = null;
    private ResultSetHandler handler = null;
    private String table = null;
    private DbUtils db = null;

    @SuppressWarnings("unused")
    private GenericDaoImplOld() {
    }
    
    @SuppressWarnings("unchecked")
    public GenericDaoImplOld(String table) {
            persistClass = (Class<T>) ((ParameterizedType) 
                            getClass().getGenericSuperclass()).getActualTypeArguments()[0];
            
            this.handler = new BeanListHandler(getPersistentClass());
            this.table = table;
            this.db = DbUtils.getInstance();
            this.qrunner = new QueryRunner(db.getDataSource());
    }
    
    protected DbUtils getDbUtils() {
    	return db;
    }
    
    protected QueryRunner getQuerryRunner() {
    	return qrunner;
    }
    
    @SuppressWarnings("unchecked")
    private List<T> runQuery(String sql) 
    throws SQLException {
            
            log.debug("Query to be executed: " + sql);

            long then = System.currentTimeMillis();
            
            List<T> list = (List<T>) qrunner.query(sql, handler);
            
            long now = System.currentTimeMillis();
            double seconds = (double) (now - then) / 1000;
            
            int length = list.size();
            
            log.debug("Query executed successfully - " 
                            + length + " records (" + seconds + " seconds).");
            
            return list;
    } 
    
    public Class<T> getPersistentClass() {
            return persistClass;
    }

    public List<T> selectAll() 
    throws Exception {
            
            String sql = "SELECT * FROM " + table;
            List<T> list = runQuery(sql);
            return list;
    }

    public List<T> selectWhere(String where) 
    throws SQLException {
            
            String sql = "SELECT * FROM " + table;
            if (where != null) {
                    sql += " WHERE " + where;
            }
            List<T> list = runQuery(sql);
            return list;
    }

    public List<T> selectLimit(long from, long to) 
    throws SQLException {
            
            String sql = "SELECT * FROM " + table 
                    + " LIMIT " + from + ", " + to; 
            List<T> list = runQuery(sql);
            return list;
    }
    
}