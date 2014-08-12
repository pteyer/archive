package edu.ucla.cs.ndntweets;

import java.util.Iterator;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import edu.ucla.cs.ndntweets.db.TweetDao;
import edu.ucla.cs.ndntweets.db.beans.Tweet;
import edu.ucla.cs.ndntweets.twitter.UserHandler;
import edu.ucla.cs.ndntweets.twitter.UserFactory;

public class Main {

	private static final Log log = LogFactory.getLog(Main.class);
	
	public static void main(String[] args) throws Exception {
		
		// set up the command socket to receive xml requests
		
		// set up the xml command parser
		
		// run the xml command parser

		
		TweetDao twdao = new TweetDao();
		UserHandler u = UserFactory.newUser("test", null);
		u.tweet("this is test tweet!");
		twdao.persist(new Tweet("testing", 0));

		List<Tweet> l = twdao.selectAll();
		Iterator<Tweet> iter = l.iterator();
		while (iter.hasNext()) {
			Tweet t = iter.next();
			log.debug(t.getText());
		}
		
//		Authentication authroizer = new Authentication();
//		Twitter tw = authroizer.authorize("peyta83");
//		
//		Random rand = new Random();
//		
//		Status status = tw.updateStatus("Testing OAuth... " + rand.nextInt());
//		log.debug("Successfully updated the status to ["
//				+ status.getText() + "].");
//		
//		List<DirectMessage> msgs = tw.getDirectMessages();
//		for (DirectMessage msg : msgs) {
//			System.out.println(msg.getText());
//		}
//	
//		List<Status> stts = tw.getFriendsTimeline();
//		for (Status stt : stts) {
//			System.out.println(stt.getUser().getName() + " -> " + stt.getText());
//			System.out.println();
//		}
//		
//		System.out.println(tw.destroyStatus(status.getId()).getText());
	}

}
