package edu.ucla.cs.ndntweets.db;

import java.util.List;

public interface GenericDaoOld<T> {

    public List<T> selectAll() throws Exception;
    public List<T> selectWhere(String where) throws Exception;
    public List<T> selectLimit(long from, long to) throws Exception;
    
}