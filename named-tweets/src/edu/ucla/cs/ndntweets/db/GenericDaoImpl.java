package edu.ucla.cs.ndntweets.db;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class GenericDaoImpl implements GenericDao {

	private static final Log log = LogFactory.getLog(GenericDaoImpl.class);
	
	private DbUtils dbutils = null;
	private Connection connection = null;
	private Statement statement = null;
	
	protected GenericDaoImpl() 
	throws SQLException {
		dbutils = DbUtils.getInstance();
		setConnection(dbutils.getSourceConnection());
		setStatement(getConnection().createStatement());
	}
	
	@Override
	public Connection getConnection() {
		return connection;
	}
	
	private void setConnection(Connection connection) {
		this.connection = connection;
	}
	
	@Override
	public Statement getStatement() {
		return statement;
	}
	
	private void setStatement(Statement statement) {
		this.statement = statement;
	}
	
}
