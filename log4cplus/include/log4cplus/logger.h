// Module:  Log4CPLUS
// File:    logger.h
// Created: 6/2001
// Author:  Tad E. Smith
//
//
// Copyright (C) Tad E. Smith  All rights reserved.
//
// This software is published under the terms of the Apache Software
// License version 1.1, a copy of which has been included with this
// distribution in the LICENSE.APL file.
//

/** @file */

#ifndef _LOG4CPLUS_LOGGERHEADER_
#define _LOG4CPLUS_LOGGERHEADER_

#include <log4cplus/config.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/helpers/pointer.h>
#include <log4cplus/spi/appenderattachable.h>
#include <log4cplus/spi/loggerfactory.h>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

namespace log4cplus {
    // Forward declarations
    namespace spi {
        struct InternalLoggingEvent;
        class LoggerImpl;
        typedef helpers::SharedObjectPtr<LoggerImpl> SharedLoggerImplPtr;
    }
    class Appender;
    class Hierarchy;
    class DefaultLoggerFactory;


    /** @var This is a list of {@link Logger Loggers}. */
    typedef std::vector<Logger> LoggerList;


    /**
     * This is the central class in the log4cplus package. One of the
     * distintive features of log4cplus are hierarchical loggers and their
     * evaluation.
     * <p>
     * See the <a href="../../../../manual.html">user manual</a> for an
     * introduction on this class.
     */
    class Logger : public log4cplus::spi::AppenderAttachable {
    public:
      // Static Methods
        /**
         * Returns <code>true </code>if the named logger exists 
         * (in the default hierarchy).
         *                
         * @param name The name of the logger to search for.
         */
        static bool exists(const std::string& name);

        /*
         * Returns all the currently defined loggers in the default
         * hierarchy.
         * <p>
         * The root logger is <em>not</em> included in the returned
         * list.     
         */
        static LoggerList getCurrentLoggers();
     
        /**
         * Return the default Hierarchy instance.
         */
        static Hierarchy& getDefaultHierarchy();

        /**
         * Retrieve a logger with name <code>name</code>.  If the named 
         * logger already exists, then the existing instance will be returned. 
         * Otherwise, a new instance is created. 
         * <p>
         * By default, loggers do not have a set LogLevel but inherit
         * it from the hierarchy. This is one of the central features of
         * log4cplus.
         * <p>
         * @param name The name of the logger to retrieve.  
         */
        static Logger getInstance(const std::string& name);

        /**
         * Like {@link #getInstance(std::string)} except that the type of logger
         * instantiated depends on the type returned by the {@link
         * spi::LoggerFactory#makeNewLoggerInstance} method of the
         * <code>factory</code> parameter.
         * <p>                         
         * This method is intended to be used by sub-classes.
         * <p>                                  
         * @param name The name of the logger to retrieve.
         * @param factory A {@link spi::LoggerFactory} implementation that will
         * actually create a new Instance.
         */
        static Logger getInstance(const std::string& name, spi::LoggerFactory& factory);

        /**
         * Return the root of the default logger hierrachy.
         * <p>
         * The root logger is always instantiated and available. It's
         * name is "root".
         * <p>
         * Nevertheless, calling {@link #getInstance
         * Logger.getInstance("root")} does not retrieve the root logger 
         * but a logger just under root named "root".
         */
        static Logger getRoot();

        /**
         * Calling this method will <em>safely</em> close and remove all
         * appenders in all the loggers including root contained in the
         * default hierachy.
         * <p>                    
         * Some appenders such as SocketAppender need to be closed before the
         * application exits. Otherwise, pending logging events might be
         * lost.
         * <p>
         * The <code>shutdown</code> method is careful to close nested
         * appenders before closing regular appenders. This is allows
         * configurations where a regular appender is attached to a logger
         * and again to a nested appender.  
         */
        static void shutdown();

      // Non-Static Methods
        /**
         * If <code>assertion</code> parameter is <code>false</code>, then
         * logs <code>msg</code> as an {@link #error(const std::string&) error} 
         * statement.
         *
         * @param assertion 
         * @param msg The message to print if <code>assertion</code> is
         * false.
         */
        void assertion(bool assertionVal, const std::string& msg) {
            if(!assertionVal) {
                log(FATAL_LOG_LEVEL, msg);
            }
        }

        /**
         * Close all attached appenders implementing the AppenderAttachable
         * interface.  
         */
        void closeNestedAppenders();

        /**
         * Check whether this logger is enabled for a given {@link
         * LogLevel} passed as parameter.
         *
         * @return boolean True if this logger is enabled for <code>ll</code>.
         */
        bool isEnabledFor(LogLevel ll) const;

        /**
         * This generic form is intended to be used by wrappers. 
         */
        void log(LogLevel ll, const std::string& message,
                 const char* file=NULL, int line=-1);

        /**
         * This method creates a new logging event and logs the event
         * without further checks.  
         */
        void forcedLog(LogLevel ll, const std::string& message,
                       const char* file=NULL, int line=-1);


        /**
         * Starting from this logger, search the logger hierarchy for a
         * "set" LogLevel and return it. Otherwise, return the LogLevel of the
         * root logger.
         * <p>
         * The Logger class is designed so that this method executes as
         * quickly as possible.
         */
        LogLevel getChainedLogLevel() const;

        /**
         * Returns the assigned {@link LogLevel}, if any, for this Logger.  
         *           
         * @return LogLevel - the assigned LogLevel, can be <code>NOT_SET_LOG_LEVEL</code>.
         */
        LogLevel getLogLevel() const;

        /**
         * Set the LogLevel of this Logger.
         * <p>
         * NULL values are admitted.
         */
        void setLogLevel(LogLevel);

        /**
         * Return the the {@link Hierarchy} where this <code>Logger</code> instance is
         * attached.
         */
        Hierarchy& getHierarchy() const;

        /**
         * Return the logger name.  
         */
        std::string getName() const;

        /**
         * Get the additivity flag for this Logger instance.  
         */
        bool getAdditivity() const;

        /**
         * Set the additivity flag for this Logger instance.
         */
        void setAdditivity(bool additive);


      // AppenderAttachable Methods
        virtual void addAppender(SharedAppenderPtr newAppender);

        virtual SharedAppenderPtrList getAllAppenders();

        virtual SharedAppenderPtr getAppender(const std::string& name);

        virtual void removeAllAppenders();

        virtual void removeAppender(SharedAppenderPtr appender);

        virtual void removeAppender(const std::string& name);

      // Copy Ctor
        Logger(const Logger& rhs);
        Logger& operator=(const Logger& rhs);

      // Dtor
        ~Logger();

        /**
         * Used to retrieve the parent of this Logger in the
         * Logger tree.
         */
        Logger getParent();

    protected:
      // Ctors
        /**
         * This constructor created a new <code>Logger</code> instance 
         * with a pointer to a Logger implementation.
         * <p>
         * It is intended to be used by sub-classes only. You should not
         * create loggers directly.
         *
         * @param ptr A pointer to the Logger implementation.  This value
         *            cannot be NULL.  
         */
        Logger(spi::LoggerImpl *ptr);
        Logger(const spi::SharedLoggerImplPtr& val);

      // Methods
        /**
         * Call the appenders in the hierrachy starting at
         * <code>this</code>.  If no appenders could be found, emit a
         * warning.
         * <p>
         * This method calls all the appenders inherited from the
         * hierarchy circumventing any evaluation of whether to log or not
         * to log the particular log request.
         *                                   
         * @param spi::InternalLoggingEvent the event to log. 
         */
        void callAppenders(const spi::InternalLoggingEvent& event);

      // Data
        /** This is a pointer to the implementation class. */
        spi::LoggerImpl *value;

    private:
      // Methods
        void init();
        void validate(const char *file, int line) const;

      // Friends
        friend class spi::LoggerImpl;
        friend class Hierarchy;
        friend class DefaultLoggerFactory;
    };


    /**
     * This class is used to create the default implementation of
     * the Logger class
     */
    class DefaultLoggerFactory : public spi::LoggerFactory {
    public:
        Logger makeNewLoggerInstance(const std::string& name, Hierarchy& h);
    };



    /**
     * This class is used to produce "Trace" logging.  When an instance of
     * this class is created, it will log a <code>"ENTER: " + msg</code>
     * log message if TRACE_LOG_LEVEL is enabled for <code>logger</code>.
     * When an instance of this class is destroyed, it will log a
     * <code>"ENTER: " + msg</code> log message if TRACE_LOG_LEVEL is enabled
     * for <code>logger</code>.
     * <p>
     * @see LOG4CPLUS_TRACE_STR
     */
    class TraceLogger {
    public:
        TraceLogger(Logger& logger, const std::string& msg) 
          : logger(logger), msg(msg) 
        { if(logger.isEnabledFor(TRACE_LOG_LEVEL))
              logger.forcedLog(TRACE_LOG_LEVEL, "ENTER: " + msg); 
        }

        ~TraceLogger()
        { if(logger.isEnabledFor(TRACE_LOG_LEVEL))
              logger.forcedLog(TRACE_LOG_LEVEL, "EXIT:  " + msg); 
        }

    private:
        Logger& logger;
        std::string msg;
    };

}; // end namespace log4cplus


#define LOG4CPLUS_TRACE_STR(logger, logEvent) \
    log4cplus::TraceLogger _log4cplus_trace_logger(logger, logEvent);

/**
 * @def LOG4CPLUS_DEBUG(logger, logEvent)  This macro is used to log a
 * DEBUG_LOG_LEVEL message to <code>logger</code>.  
 * <code>log</code> will be streamed into an <code>ostream</code>.
 * 
 */
#define LOG4CPLUS_DEBUG(logger, logEvent) \
    if(logger.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)) { \
        std::ostringstream _log4cplus_buf; \
        _log4cplus_buf << logEvent; \
        logger.forcedLog(log4cplus::DEBUG_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
    }
#define LOG4CPLUS_DEBUG_STR(logger, logEvent) \
    logger.log(log4cplus::DEBUG_LOG_LEVEL, logEvent, __FILE__, __LINE__); \

#define LOG4CPLUS_INFO(logger, logEvent) \
    if(logger.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) { \
        std::ostringstream _log4cplus_buf; \
        _log4cplus_buf << logEvent; \
        logger.forcedLog(log4cplus::INFO_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
    }
#define LOG4CPLUS_INFO_STR(logger, logEvent) \
    logger.log(INFO_LOG_LEVEL, logEvent, __FILE__, __LINE__); \

#define LOG4CPLUS_WARN(logger, logEvent) \
    if(logger.isEnabledFor(log4cplus::WARN_LOG_LEVEL)) { \
        std::ostringstream _log4cplus_buf; \
        _log4cplus_buf << logEvent; \
        logger.forcedLog(log4cplus::WARN_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
    }
#define LOG4CPLUS_WARN_STR(logger, logEvent) \
    logger.log(log4cplus::WARN_LOG_LEVEL, logEvent, __FILE__, __LINE__); \

#define LOG4CPLUS_ERROR(logger, logEvent) \
    if(logger.isEnabledFor(log4cplus::ERROR_LOG_LEVEL)) { \
        std::ostringstream _log4cplus_buf; \
        _log4cplus_buf << logEvent; \
        logger.forcedLog(log4cplus::ERROR_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
    }
#define LOG4CPLUS_ERROR_STR(logger, logEvent) \
    logger.log(log4cplus::ERROR_LOG_LEVEL, logEvent, __FILE__, __LINE__); \

#define LOG4CPLUS_FATAL(logger, logEvent) \
    if(logger.isEnabledFor(log4cplus::FATAL_LOG_LEVEL)) { \
        std::ostringstream _log4cplus_buf; \
        _log4cplus_buf << logEvent; \
        logger.forcedLog(log4cplus::FATAL_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
    }
#define LOG4CPLUS_FATAL_STR(logger, logEvent) \
    logger.log(log4cplus::FATAL_LOG_LEVEL, logEvent, __FILE__, __LINE__); \


#endif // _LOG4CPLUS_LOGGERHEADER_

