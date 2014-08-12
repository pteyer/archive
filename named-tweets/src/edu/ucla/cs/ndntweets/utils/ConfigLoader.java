package edu.ucla.cs.ndntweets.utils;

import org.apache.commons.configuration.ConfigurationException;
import org.apache.commons.configuration.XMLConfiguration;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class ConfigLoader {
    private static final Log log = LogFactory.getLog(ConfigLoader.class);

    private static ConfigLoader instance = null;
    
    private XMLConfiguration config = null;

    private ConfigLoader() {
            config = new XMLConfiguration();
            
            log.debug("Loading configuration file at: " + Constants.PATH_CONFIG_FILE);

            try {
                    config.load(Constants.PATH_CONFIG_FILE);
            } catch (ConfigurationException e) {
                    log.fatal("Unable to load configuration file: " + Constants.PATH_CONFIG_FILE);
                    System.exit(-1);
            }
    }
    
    public static ConfigLoader getInstance() {
            if (instance == null) {
                    instance = new ConfigLoader();
            }
            return instance;
    }
    
    public XMLConfiguration getConfig() {
            return config;
    }
    
    public String getRequiredString(String key) {
            String res = null;
            try {
                    res = config.getString(key);
            } catch (Exception e) {
                    log.warn("Unable to load configuration for: " + key);
            } finally {
                    if (res == null) {
                            log.fatal("Missing required configuration key: \"" + key + "\"");
                            System.exit(-1);
                    }
            }
            return res;
    }
    
    public String[] getRequiredStringArray(String key) {
            String[] res = null;
            try {
                    res = config.getStringArray(key);
            } catch (Exception e) {
                    log.warn("Unable to load configuration for: " + key);
            } finally {
                    if (res == null || res.length == 0) {
                            log.fatal("Missing required configuration key: \"" + key + "\"");
                            System.exit(-1);
                    }
            }
            return res;
    }
}
