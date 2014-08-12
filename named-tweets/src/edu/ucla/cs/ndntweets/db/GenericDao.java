package edu.ucla.cs.ndntweets.db;

import java.sql.Connection;
import java.sql.Statement;

public interface GenericDao {

	public Connection getConnection();
	public Statement getStatement();
	
}
