
(cl:in-package :asdf)

(defsystem "all_msgs_srvs-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "settingcallback" :depends-on ("_package_settingcallback"))
    (:file "_package_settingcallback" :depends-on ("_package"))
  ))