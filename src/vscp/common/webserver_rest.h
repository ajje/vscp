// webserver_rest.h: 
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version
// 2 of the License, or (at your option) any later version.
//
// This file is part of the VSCP (http://www.vscp.org)
//
// Copyright (C) 2000-2016 
// Ake Hedman, Grodans Paradis AB, <akhe@grodansparadis.com>
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//

#if !defined(WEBSERVER_REST_H____INCLUDED_)
#define WEBSERVER_REST_H____INCLUDED_



//******************************************************************************
//                                      REST
//******************************************************************************


/**
 * Session varaibles we keep for each user/session/browser.
 */
struct websrv_rest_session
{
    // We keep all sessions in a linked list.
    struct websrv_rest_session *m_next;

    // Unique ID for this session. 
    char sid[33];

    // Time when this session was last active.
    time_t lastActiveTime;

    // Client item for this session
    CClientItem *pClientItem;

    // User
    CUserItem *pUserItem;

    // Remote IP
    char remote_ip[48];
      
};

enum {
    REST_ERROR_CODE_SUCCESS = 0,
    REST_ERROR_CODE_GENERAL_FAILURE,
    REST_ERROR_CODE_INVALID_SESSION ,
    REST_ERROR_CODE_UNSUPPORTED_FORMAT,
    REST_ERROR_CODE_COULD_NOT_OPEN_SESSION,
    REST_ERROR_CODE_MISSING_DATA,
    RESR_ERROR_CODE_INPUT_QUEUE_EMPTY,
    REST_ERROR_CODE_VARIABLE_NOT_FOUND,
    REST_ERROR_CODE_VARIABLE_NOT_CREATED,
    REST_ERROR_CODE_NO_ROOM,
    REST_ERROR_CODE_TABLE_NOT_FOUND,
    REST_ERROR_CODE_TABLE_NO_DATA,
    REST_ERROR_CODE_TABLE_RANGE,
    REST_ERROR_CODE_INVALID_USER,
    REST_ERROR_CODE_INVALID_ORIGIN,
    REST_ERROR_CODE_INVALID_PASSWORD,
};

// REST formats
enum {
    REST_FORMAT_PLAIN = 0,
    REST_FORMAT_CSV,            // http://tools.ietf.org/html/rfc4180
    REST_FORMAT_XML,
    REST_FORMAT_JSON,
    REST_FORMAT_JSONP,
    REST_FORMAT_COUNT
};

enum {
    REST_SUCCESS_CODE_SUCCESS = 1,  // All is OK                message="succss"
    REST_SUCCESS_CODE_INFO,         // This is info             message="info"
    REST_SUCCESS_CODE_DATA,         // This is data             message="data"
    REST_SUCCESS_CODE_EVENT_COUNT,  // This is event count      message="count"
};

#define REST_MIME_TYPE_PLAIN        "text/plain"
#define REST_MIME_TYPE_CSV          "text/csv"
#define REST_MIME_TYPE_XML          "application/xml"
#define REST_MIME_TYPE_JSON         "application/json"
#define REST_MIME_TYPE_JSONP        "application/javascript"

// Clear text Error messages
#define REST_PLAIN_ERROR_SUCCESS                "1 1 Success \r\n\r\nEverything is fine.\r\n"
#define REST_PLAIN_ERROR_GENERAL_FAILURE        "0 -1 Failure \r\n\r\nGeneral failure.\r\n"
#define REST_PLAIN_ERROR_INVALID_SESSION        "0 -2 Invalid session \r\n\r\nThe session must be opened with 'open' before a session command can be used. It may also be possible that the session has timed out.\r\n"
#define REST_PLAIN_ERROR_UNSUPPORTED_FORMAT     "0 -3 Unsupported format \r\n\r\nFormat can be 0/plain, 1/CSV, 2/XML, 3/JSON, 4/JSONP´.\r\n"
#define REST_PLAIN_ERROR_COULD_NOT_OPEN_SESSION "0 -4 Unable to create session \r\n\r\nA new session could not be created.\r\n"
#define REST_PLAIN_ERROR_MISSING_DATA           "0 -5 Missing data/parameter \r\n\r\nA needed parameter or data is missing to be able to perform operation.\r\n"
#define REST_PLAIN_ERROR_INPUT_QUEUE_EMPTY      "0 -6 Input queue empty \r\n\r\nThe input queue is empty.\r\n"
#define REST_PLAIN_ERROR_VARIABLE_NOT_FOUND     "0 -7 Variable not found \r\n\r\nVariable could not be found.\r\n"
#define REST_PLAIN_ERROR_VARIABLE_NOT_CREATED   "0 -8 Variable could not be created \r\n\r\nVariable could not be created.\r\n"
#define REST_PLAIN_ERROR_NO_ROOM                "0 -9 No room in queue \r\n\r\nNo room in queue.\r\n"
#define REST_PLAIN_ERROR_TABLE_NOT_FOUND        "0 -10 Table does not exist \r\n\r\nA table with that name does not exist.\r\n"
#define REST_PLAIN_ERROR_TABLE_NO_DATA          "0 -11 No data \r\n\r\nThe table exist but contains no data (in that range).\r\n"
#define REST_PLAIN_ERROR_TABLE_RANGE            "0 -12 Range error \r\n\r\nThe end date must be later than the start date.\r\n"
#define REST_PLAIN_ERROR_INVALID_USER           "0 -13 Invalid user \r\n\r\nThe user is not a valid user of this system.\r\n"
#define REST_PLAIN_ERROR_INVALID_ORIGIN         "0 -14 Invalid origin \r\n\r\nYou are not allowed to connect from his location.\r\n"
#define REST_PLAIN_ERROR_INVALID_PASSWORD       "0 -15 Invalid passsword \r\n\r\nWrong credentials has been given.\r\n"

#define REST_CSV_ERROR_SUCCESS                  "success-code,error-code,message,description\r\n1,1,Success,Success."
#define REST_CSV_ERROR_GENERAL_FAILURE          "success-code,error-code,message,description\r\n0,-1,Failure,General failure."
#define REST_CSV_ERROR_INVALID_SESSION          "success-code,error-code,message,description\r\n0,-2,Invalid session,The session must be opened with 'open' before a session command can be used. It may also be possible that the session has timed out."
#define REST_CSV_ERROR_UNSUPPORTED_FORMAT       "success-code,error-code,message,description\r\n0,-3,Unsupported format,Format can be 0/plain, 1/CSV, 2/XML, 3/JSON, 4/JSONP."
#define REST_CSV_ERROR_COULD_NOT_OPEN_SESSION   "success-code,error-code,message,description\r\n0,-4,Unable to create session,A new session could not be created."
#define REST_CSV_ERROR_MISSING_DATA             "success-code,error-code,message,description\r\n0,-5,Missing data/parameter,A needed parameter or data is missing to be able to perform operation."
#define REST_CSV_ERROR_INPUT_QUEUE_EMPTY        "success-code,error-code,message,description\r\n0,-6,Input queue empty,The input queue is empty."
#define REST_CSV_ERROR_VARIABLE_NOT_FOUND       "success-code,error-code,message,description\r\n0,-7,Variable not found,Variable could not be found."
#define REST_CSV_ERROR_VARIABLE_NOT_CREATED     "success-code,error-code,message,description\r\n0,-8,Variable could not be created,Variable could not be created."
#define REST_CSV_ERROR_NO_ROOM                  "success-code,error-code,message,description\r\n0,-9,No room in queue,No room in queue."
#define REST_CSV_ERROR_TABLE_NOT_FOUND          "success-code,error-code,message,description\r\n0,-10,Table does not exist,A table with that name does not exist."
#define REST_CSV_ERROR_TABLE_NO_DATA            "success-code,error-code,message,description\r\n0,-11,No data ,The table exist but contains no data (in that range)."
#define REST_CSV_ERROR_TABLE_RANGE              "success-code,error-code,message,description\r\n0,-12,Range error ,The end date must be later than the start date."
#define REST_CSV_ERROR_INVALID_USER             "success-code,error-code,message,description\r\n0,-13,Invalid user ,The user is not a valid user of this system."
#define REST_CSV_ERROR_INVALID_ORIGIN           "success-code,error-code,message,description\r\n0,-14,Invalid origin ,You are not allowed to connect from his location."
#define REST_CSV_ERROR_INVALID_PASSWORD         "success-code,error-code,message,description\r\n0,-15,Invalid password ,Wrong credentials has been given."

#define XML_HEADER  "<?xml version = \"1.0\" encoding = \"UTF-8\" ?>"
#define REST_XML_ERROR_SUCCESS                  "<vscp-rest success = \"true\" code = \"1\" message = \"Success\" description = \"Success.\" />"
#define REST_XML_ERROR_GENERAL_FAILURE          "<vscp-rest success = \"false\" code = \"-1\" message = \"Failure\" description = \"General failure.\" />"
#define REST_XML_ERROR_INVALID_SESSION          "<vscp-rest success = \"false\" code = \"-2\" message = \"Invalid session\" description = \"The session must be opened with 'open' before a session command can be used. It may also be possible that the session has timed out.\" />"
#define REST_XML_ERROR_UNSUPPORTED_FORMAT       "<vscp-rest success = \"false\" code = \"-3\" message = \"Unsupported format\" description = \"Format can be 0/plain, 1/CSV, 2/XML, 3/JSON, 4/JSONP´.\" />"
#define REST_XML_ERROR_COULD_NOT_OPEN_SESSION   "<vscp-rest success = \"false\" code = \"-4\" message = \"Unable to create session\" description = \"A new session could not be created.\" />"
#define REST_XML_ERROR_MISSING_DATA             "<vscp-rest success = \"false\" code = \"-5\" message = \"Missing data/parameter\" description = \"A needed parameter or data is missing to be able to perform operation.\" />"
#define REST_XML_ERROR_INPUT_QUEUE_EMPTY        "<vscp-rest success = \"false\" code = \"-6\" message = \"Input queue empty\" description = \"The input queue is empty.\" />"
#define REST_XML_ERROR_VARIABLE_NOT_FOUND       "<vscp-rest success = \"false\" code = \"-7\" message = \"Variable not found\" description = \"Variable could not be found.\" />"
#define REST_XML_ERROR_VARIABLE_NOT_CREATED     "<vscp-rest success = \"false\" code = \"-8\" message = \"Variable could not be created\" description = \"Variable could not be created.\" />"
#define REST_XML_ERROR_NO_ROOM                  "<vscp-rest success = \"false\" code = \"-9\" message = \"No room in queue\" description = \"No room in queue.\" />"
#define REST_XML_ERROR_TABLE_NOT_FOUND          "<vscp-rest success = \"false\" code = \"-10\" message = \"Table does not exist\" description = \"A table with that name does not exist.\" />"
#define REST_XML_ERROR_TABLE_NO_DATA            "<vscp-rest success = \"false\" code = \"-11\" message = \"No data\" description = \"The table exist but contains no data (in that range).\" />"
#define REST_XML_ERROR_TABLE_RANGE              "<vscp-rest success = \"false\" code = \"-12\" message = \"Range error\" description = \"The end date must be later than the start date.\" />"
#define REST_XML_ERROR_INVALID_USER             "<vscp-rest success = \"false\" code = \"-13\" message = \"Invalid user\" description = \"The user is not a valid user of this system.\" />"
#define REST_XML_ERROR_INVALID_ORIGIN           "<vscp-rest success = \"false\" code = \"-14\" message = \"Invalid origin\" description = \"You are not allowed to connect from his location.\" />"
#define REST_XML_ERROR_INVALID_PASSWORD         "<vscp-rest success = \"false\" code = \"-15\" message = \"Invalid password\" description = \"Wrong credentials has been given.\" />"

#define REST_JSON_ERROR_SUCCESS                 "{\"success\":true,\"code\":1,\"message\":\"success\",\"description\":\"Success\"}"
#define REST_JSON_ERROR_GENERAL_FAILURE         "{\"success\":false,\"code\":-1,\"message\":\"Failure\",\"description\":\"General failure.\"}"
#define REST_JSON_ERROR_INVALID_SESSION         "{\"success\":false,\"code\":-2,\"message\":\"Invalid session\",\"description\":\"The session must be opened with 'open' before a session command can be used. It may also be possible that the session has timed out.\"}"
#define REST_JSON_ERROR_UNSUPPORTED_FORMAT      "{\"success\":false,\"code\":-3,\"message\":\"Unsupported format\",\"description\":\"Unsupported format. Format can be 0/plain, 1/CSV, 2/XML, 3/JSON, 4/JSONP´.\"}"
#define REST_JSON_ERROR_COULD_NOT_OPEN_SESSION  "{\"success\":false,\"code\":-4,\"message\":\"Unable to create session\",\"description\":\"A new session could not be created.\"}"
#define REST_JSON_ERROR_MISSING_DATA            "{\"success\":false,\"code\":-5,\"message\":\"Missing data/parameter\",\"description\":\"A needed parameter or data is missing to be able to perform operation.\"}"
#define REST_JSON_ERROR_INPUT_QUEUE_EMPTY       "{\"success\":false,\"code\":-6,\"message\":\"Input queue empty\",\"description\":\"The input queue is empty.\"}"
#define REST_JSON_ERROR_VARIABLE_NOT_FOUND      "{\"success\":false,\"code\":-7,\"message\":\"Variable not found\",\"description\":\"Variable could not be found.\"}"
#define REST_JSON_ERROR_VARIABLE_NOT_CREATED    "{\"success\":false,\"code\":-8,\"message\":\"Variable could not be created\",\"description\":\"Variable could not be created.\"}"
#define REST_JSON_ERROR_NO_ROOM	                "{\"success\":false,\"code\":-9,\"message\":\"No room in queue\",\"description\":\"No room in queue.\"}"
#define REST_JSON_ERROR_TABLE_NOT_FOUND         "{\"success\":false,\"code\":-10,\"message\":\"Table does not exist\",\"description\":\"A table with that name does not exist.\"}"
#define REST_JSON_ERROR_TABLE_NO_DATA           "{\"success\":false,\"code\":-11,\"message\":\"No data\",\"description\":\"The table exist but contains no data (in that range).\"}"
#define REST_JSON_ERROR_TABLE_RANGE             "{\"success\":false,\"code\":-12,\"message\":\"Range error\",\"description\":\"The end date must be later than the start date.\"}"
#define REST_JSON_ERROR_INVALID_USER            "{\"success\":false,\"code\":-13,\"message\":\"Invalid user\",\"description\":\"The user is not a valid user of this system.\"}"
#define REST_JSON_ERROR_INVALID_ORIGIN          "{\"success\":false,\"code\":-14,\"message\":\"Invalid origin\",\"description\":\"You are not allowed to connect from his location.\"}"
#define REST_JSON_ERROR_INVALID_PASSWORD        "{\"success\":false,\"code\":-15,\"message\":\"Invalid password\",\"description\":\"Wrong credentials has been given.\"}"

#define REST_JSONP_ERROR_SUCCESS                "typeof handler === 'function' && handler(" REST_JSON_ERROR_SUCCESS ");"
#define REST_JSONP_ERROR_GENERAL_FAILURE        "typeof handler === 'function' && handler(" REST_JSON_ERROR_GENERAL_FAILURE ");"
#define REST_JSONP_ERROR_INVALID_SESSION        "typeof handler === 'function' && handler(" REST_JSON_ERROR_INVALID_SESSION ");"
#define REST_JSONP_ERROR_UNSUPPORTED_FORMAT     "typeof handler === 'function' && handler(" REST_JSON_ERROR_UNSUPPORTED_FORMAT ");"
#define REST_JSONP_ERROR_COULD_NOT_OPEN_SESSION "typeof handler === 'function' && handler(" REST_JSON_ERROR_COULD_NOT_OPEN_SESSION ");"
#define REST_JSONP_ERROR_MISSING_DATA           "typeof handler === 'function' && handler(" REST_JSON_ERROR_MISSING_DATA ");"
#define REST_JSONP_ERROR_INPUT_QUEUE_EMPTY      "typeof handler === 'function' && handler(" REST_JSON_ERROR_INPUT_QUEUE_EMPTY ");"
#define REST_JSONP_ERROR_VARIABLE_NOT_FOUND     "typeof handler === 'function' && handler(" REST_JSON_ERROR_VARIABLE_NOT_FOUND ");"
#define REST_JSONP_ERROR_VARIABLE_NOT_CREATED   "typeof handler === 'function' && handler(" REST_JSON_ERROR_VARIABLE_NOT_CREATED ");"
#define REST_JSONP_ERROR_NO_ROOM                "typeof handler === 'function' && handler(" REST_JSON_ERROR_NO_ROOM ");"
#define REST_JSONP_ERROR_TABLE_NOT_FOUND        "typeof handler === 'function' && handler(" REST_JSON_ERROR_TABLE_NOT_FOUND ");"
#define REST_JSONP_ERROR_TABLE_NO_DATA          "typeof handler === 'function' && handler(" REST_JSON_ERROR_TABLE_NO_DATA ");"
#define REST_JSONP_ERROR_TABLE_RANGE            "typeof handler === 'function' && handler(" REST_JSON_ERROR_TABLE_RANGE ");"
#define REST_JSONP_ERROR_INVALID_USER           "typeof handler === 'function' && handler(" REST_JSON_ERROR_INVALID_USER ");"
#define REST_JSONP_ERROR_INVALID_ORIGIN         "typeof handler === 'function' && handler(" REST_JSON_ERROR_INVALID_ORIGIN ");"
#define REST_JSONP_ERROR_INVALID_PASSWORD       "typeof handler === 'function' && handler(" REST_JSON_ERROR_INVALID_PASSWORD ");"

const char* rest_errors[][REST_FORMAT_COUNT+1] = {
    {REST_PLAIN_ERROR_SUCCESS, REST_CSV_ERROR_SUCCESS,REST_XML_ERROR_SUCCESS,REST_JSON_ERROR_SUCCESS,REST_JSONP_ERROR_SUCCESS,REST_JSONP_ERROR_SUCCESS},
    {REST_PLAIN_ERROR_GENERAL_FAILURE,REST_CSV_ERROR_GENERAL_FAILURE,REST_XML_ERROR_GENERAL_FAILURE,REST_JSON_ERROR_GENERAL_FAILURE,REST_JSONP_ERROR_GENERAL_FAILURE,REST_JSONP_ERROR_GENERAL_FAILURE,},
    {REST_PLAIN_ERROR_INVALID_SESSION,REST_CSV_ERROR_INVALID_SESSION,REST_XML_ERROR_INVALID_SESSION,REST_JSON_ERROR_INVALID_SESSION,REST_JSONP_ERROR_INVALID_SESSION,REST_JSONP_ERROR_INVALID_SESSION,},
    {REST_PLAIN_ERROR_UNSUPPORTED_FORMAT,REST_CSV_ERROR_UNSUPPORTED_FORMAT,REST_XML_ERROR_UNSUPPORTED_FORMAT,REST_JSON_ERROR_UNSUPPORTED_FORMAT,REST_JSONP_ERROR_UNSUPPORTED_FORMAT,REST_JSONP_ERROR_UNSUPPORTED_FORMAT,},
    {REST_PLAIN_ERROR_COULD_NOT_OPEN_SESSION,REST_CSV_ERROR_COULD_NOT_OPEN_SESSION,REST_XML_ERROR_COULD_NOT_OPEN_SESSION,REST_JSON_ERROR_COULD_NOT_OPEN_SESSION,REST_JSONP_ERROR_COULD_NOT_OPEN_SESSION,REST_JSONP_ERROR_COULD_NOT_OPEN_SESSION,},
    {REST_PLAIN_ERROR_MISSING_DATA,REST_CSV_ERROR_MISSING_DATA,REST_XML_ERROR_MISSING_DATA,REST_JSON_ERROR_MISSING_DATA,REST_JSONP_ERROR_MISSING_DATA,REST_JSONP_ERROR_MISSING_DATA},
    {REST_PLAIN_ERROR_INPUT_QUEUE_EMPTY,REST_CSV_ERROR_INPUT_QUEUE_EMPTY,REST_XML_ERROR_INPUT_QUEUE_EMPTY,REST_JSON_ERROR_INPUT_QUEUE_EMPTY,REST_JSONP_ERROR_INPUT_QUEUE_EMPTY,REST_JSONP_ERROR_INPUT_QUEUE_EMPTY},
    {REST_PLAIN_ERROR_VARIABLE_NOT_FOUND,REST_CSV_ERROR_VARIABLE_NOT_FOUND,REST_XML_ERROR_VARIABLE_NOT_FOUND,REST_JSON_ERROR_VARIABLE_NOT_FOUND,REST_JSONP_ERROR_VARIABLE_NOT_FOUND },
    {REST_PLAIN_ERROR_VARIABLE_NOT_CREATED,REST_CSV_ERROR_VARIABLE_NOT_CREATED,REST_XML_ERROR_VARIABLE_NOT_CREATED,REST_JSON_ERROR_VARIABLE_NOT_CREATED,REST_JSONP_ERROR_VARIABLE_NOT_CREATED },
    {REST_PLAIN_ERROR_NO_ROOM,REST_CSV_ERROR_NO_ROOM,REST_XML_ERROR_NO_ROOM,REST_JSON_ERROR_NO_ROOM,REST_JSONP_ERROR_NO_ROOM,REST_JSONP_ERROR_NO_ROOM },
    {REST_PLAIN_ERROR_TABLE_NOT_FOUND,REST_CSV_ERROR_TABLE_NOT_FOUND,REST_XML_ERROR_TABLE_NOT_FOUND,REST_JSON_ERROR_TABLE_NOT_FOUND,REST_JSONP_ERROR_TABLE_NOT_FOUND,REST_JSONP_ERROR_TABLE_NOT_FOUND },
    {REST_PLAIN_ERROR_TABLE_NO_DATA,REST_CSV_ERROR_TABLE_NO_DATA,REST_XML_ERROR_TABLE_NO_DATA,REST_JSON_ERROR_TABLE_NO_DATA,REST_JSONP_ERROR_TABLE_NO_DATA,REST_JSONP_ERROR_TABLE_NO_DATA },
    {REST_PLAIN_ERROR_TABLE_RANGE,REST_CSV_ERROR_TABLE_RANGE,REST_XML_ERROR_TABLE_RANGE,REST_JSON_ERROR_TABLE_RANGE,REST_JSONP_ERROR_TABLE_RANGE,REST_JSONP_ERROR_TABLE_RANGE },
    {REST_PLAIN_ERROR_INVALID_USER,REST_CSV_ERROR_INVALID_USER,REST_XML_ERROR_INVALID_USER,REST_JSON_ERROR_INVALID_USER,REST_JSONP_ERROR_INVALID_USER,REST_JSONP_ERROR_INVALID_USER },
    {REST_PLAIN_ERROR_INVALID_ORIGIN,REST_CSV_ERROR_INVALID_ORIGIN,REST_XML_ERROR_INVALID_ORIGIN,REST_JSON_ERROR_INVALID_ORIGIN,REST_JSONP_ERROR_INVALID_ORIGIN,REST_JSONP_ERROR_INVALID_ORIGIN },
    {REST_PLAIN_ERROR_INVALID_PASSWORD,REST_CSV_ERROR_INVALID_PASSWORD,REST_XML_ERROR_INVALID_PASSWORD,REST_JSON_ERROR_INVALID_PASSWORD,REST_JSONP_ERROR_INVALID_PASSWORD,REST_JSONP_ERROR_INVALID_PASSWORD },
};

#endif