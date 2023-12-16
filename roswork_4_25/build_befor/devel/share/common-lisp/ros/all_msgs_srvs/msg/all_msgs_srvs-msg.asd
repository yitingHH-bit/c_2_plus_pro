
(cl:in-package :asdf)

(defsystem "all_msgs_srvs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "bdinfomsg" :depends-on ("_package_bdinfomsg"))
    (:file "_package_bdinfomsg" :depends-on ("_package"))
    (:file "errorMsgs" :depends-on ("_package_errorMsgs"))
    (:file "_package_errorMsgs" :depends-on ("_package"))
    (:file "settings" :depends-on ("_package_settings"))
    (:file "_package_settings" :depends-on ("_package"))
    (:file "systemState" :depends-on ("_package_systemState"))
    (:file "_package_systemState" :depends-on ("_package"))
    (:file "systemlog" :depends-on ("_package_systemlog"))
    (:file "_package_systemlog" :depends-on ("_package"))
    (:file "vehicalmsgs" :depends-on ("_package_vehicalmsgs"))
    (:file "_package_vehicalmsgs" :depends-on ("_package"))
  ))