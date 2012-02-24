package com.gapso.trieda.hibernate;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.hibernate.HibernateException;
import org.hibernate.event.EventSource;
import org.hibernate.event.def.DefaultFlushEventListener;

/**
 * Patches Hibernate to prevent this issue
 * http://opensource.atlassian.com/projects/hibernate/browse/HHH-2763
 *
 * TODO: Remove this patch when HHH-2763 is resolved
 *
 * @author Graeme Rocher
 * @since 1.2
 */
public class PatchedDefaultFlushEventListener extends DefaultFlushEventListener{

    private static final long serialVersionUID = 1;
    private static final Log LOG = LogFactory.getLog(PatchedDefaultFlushEventListener.class);

    @Override
    protected void performExecutions(EventSource session) throws HibernateException {
        session.getPersistenceContext().setFlushing(true);
        try {
            session.getJDBCContext().getConnectionManager().flushBeginning();
            // we need to lock the collection caches before
            // executing entity inserts/updates in order to
            // account for bidi associations
            session.getActionQueue().prepareActions();
            session.getActionQueue().executeActions();
        }
        catch (HibernateException he) {
            LOG.error("Could not synchronize database state with session", he);
            throw he;
        }
        finally {
            session.getPersistenceContext().setFlushing(false);
            session.getJDBCContext().getConnectionManager().flushEnding();
        }
    }
}