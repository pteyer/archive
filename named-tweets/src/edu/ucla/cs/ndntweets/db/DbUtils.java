package edu.ucla.cs.ndntweets.db;

import java.sql.Connection;

import org.apache.commons.dbcp.BasicDataSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import edu.ucla.cs.ndntweets.utils.ConfigLoader;
import edu.ucla.cs.ndntweets.utils.Constants;

public class DbUtils {

	private static final Log log = LogFactory.getLog(DbUtils.class);

	private static DbUtils instance = null;
    
    private String sourceUrl = null;
    private String sourceDriver = null;
    private String sourceUsername = null;
    private String sourcePasswd = null;
    
    private BasicDataSource sourceDs = null;
    
    private Connection sourceConn = null;
    
    private DbUtils() {
            ConfigLoader config = ConfigLoader.getInstance();
            
            sourceDriver = config.getRequiredString(Constants.TAGS_DATABASE_DRIVER);
            sourceUrl = config.getRequiredString(Constants.TAGS_DATABASE_URL);
            sourceUsername = config.getRequiredString(Constants.TAGS_DATABASE_USERNAME);
            sourcePasswd = config.getRequiredString(Constants.TAGS_DATABASE_PASSWORD);

            sourceDs = new BasicDataSource();
            
            sourceDs.setDriverClassName(sourceDriver);
            sourceDs.setUrl(sourceUrl);
            sourceDs.setUsername(sourceUsername);
            sourceDs.setPassword(sourcePasswd);
            
    }
    
    public static DbUtils getInstance() {
            if (instance == null) {
                    instance = new DbUtils();
            }
            return instance;
    }
    
    public BasicDataSource getDataSource() {
            return sourceDs;
    }

    public Connection getSourceConnection() {
            try {
                    if (sourceConn == null 
                                    || (! sourceConn.isValid(0))) {
                            sourceConn = getDataSource().getConnection(); 
                    }
                    return sourceConn;
            } catch (Exception e) {
                    log.error("Unable to connect to source database: " + e.getMessage());
                    return null;
            }
    }

}
