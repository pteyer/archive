package edu.ucla.cs.ndntweets.db;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import edu.ucla.cs.ndntweets.db.beans.Tweet;

public class TweetDao extends GenericDaoImpl {

	public static final String TWEETS_TABLE = "tweets";

	private static final Log log = LogFactory.getLog(TweetDao.class);
	
	private PreparedStatement prestmt = null;
	
	public TweetDao() 
	throws SQLException {
//		super(TWEETS_TABLE);
		super();

		try {
			createTable();
		} catch (SQLException ignore) {
			log.info("Unable to create table " + TWEETS_TABLE 
					+ ": " + ignore.getMessage());
		}
		
		String stmtstr = "INSERT INTO " + TWEETS_TABLE + " (userid, text) VALUES (?, ?)";
		prestmt = getConnection().prepareStatement(stmtstr);
	}

	public void createTable() 
	throws SQLException {
		
		getStatement().executeUpdate(
				"CREATE TABLE " + TWEETS_TABLE + " (" +
				"id INTEGER GENERATED ALWAYS AS IDENTITY (START WITH 0, INCREMENT BY 1)," +
				"userid INTEGER NOT NULL," +
				"text CHAR(141))");
	}
	
	public void persist(Tweet tweet) 
	throws SQLException {
		prestmt.setInt(1, tweet.getUserId());
		prestmt.setString(2, tweet.getText());
		prestmt.executeUpdate();
		getConnection().commit();
		
		log.debug("Inserted successfully");
	}
	
	public List<Tweet> selectAll() 
	throws SQLException {
		List<Tweet> result = new LinkedList<Tweet>();
		
		String sql = "SELECT id, text, userid FROM " + TWEETS_TABLE;
		ResultSet rs = getStatement().executeQuery(sql);
		
		while (rs.next()) {
			Tweet t = new Tweet(rs.getInt(1), rs.getString(2), rs.getInt(3));
			result.add(t);
		}
		
		log.debug("Retrieved " + result.size() + " records from database");
		
		return result;
	}
}
