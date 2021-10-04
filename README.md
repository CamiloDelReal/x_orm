# XOrm
Qt object relational mapping library

<img src="https://github.com/CamiloDelReal/x_orm/blob/develop/artwork/logo.svg" />

### Features
- Database entities
- Entity foreign relationships
- Foreign relations resolution
- Data converter for non native data type

### Future features
- Create join clauses

### Libraries
- Qt Framework Open Source
  * [Qt Framework](https://www.qt.io/)

### Examples

#### Demos
- [Console](https://github.com/CamiloDelReal/xorm_local_demo)
- [One Entity with UI](https://github.com/CamiloDelReal/xorm_local_demo_one)
- [One to One relationship with UI](https://github.com/CamiloDelReal/xorm_local_demo_one_to_one)
- [One to Many relationship with UI](https://github.com/CamiloDelReal/xorm_local_demo_one_to_many)
- [Many to Many relationship with UI](https://github.com/CamiloDelReal/xorm_local_demo_many_to_many)

#### One Entity
<img src="https://github.com/CamiloDelReal/x_orm/blob/develop/design/one_entity.jpg" width="50%" height="50%" />

    class Member : public QObject, public XTableModel<Member>
    {
        Q_OBJECT

        Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString job READ job WRITE setJob NOTIFY jobChanged)

        X_TABLE_NAME("members")
        X_TABLE_FIELDS(X_FIELD(guid, X_PRIMARY_KEY),
                    X_FIELD(name),
                    X_FIELD(job))

    public:
        explicit Member(QObject *parent = nullptr);
        Member(const quint64 &guid, const QString &name, const QString &job, QObject *parent = nullptr);
        Member(const QString &name, const QString &job, QObject *parent = nullptr);
        Member(const Member &other);

        Member& operator=(const Member &other);

        quint64 guid() const;
        QString name() const;
        QString job() const;

    public slots:
        void setGuid(const quint64 &guid);
        void setName(const QString &name);
        void setJob(const QString &job);

    signals:
        void guidChanged(const quint64 &guid);
        void nameChanged(const QString &name);
        void jobChanged(const QString &job);

    private:
        quint64 m_guid;
        QString m_name;
        QString m_job;
    };


#### One to One
<img src="https://github.com/CamiloDelReal/x_orm/blob/develop/design/one_to_one.jpg" width="80%" height="80%" />

    class Department : public QObject, public XTableModel<Department>
    {
        Q_OBJECT

        Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

        X_TABLE_NAME("departments")
        X_TABLE_FIELDS(X_FIELD(guid, X_PRIMARY_KEY),
                    X_FIELD(name),
                    X_FIELD(description))

    public:
        explicit Department(QObject *parent = nullptr);
        Department(const quint64 &guid, const QString &name, const QString &description, QObject *parent = nullptr);
        Department(const QString &name, const QString &description, QObject *parent = nullptr);
        Department(const Department &other);

        Department& operator=(const Department &other);

        quint64 guid() const;
        QString name() const;
        QString description() const;

    public slots:
        void setGuid(const quint64 &guid);
        void setName(const QString &name);
        void setDescription(const QString &description);

    signals:
        void guidChanged(const quint64 &guid);
        void nameChanged(const QString &name);
        void descriptionChanged(const QString &description);

    private:
        quint64 m_guid;
        QString m_name;
        QString m_description;
    };

    class Member : public QObject, public XTableModel<Member>
    {
        Q_OBJECT

        Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString job READ job WRITE setJob NOTIFY jobChanged)
        Q_PROPERTY(quint64 departmentId READ departmentId WRITE setDepartmentId NOTIFY departmentIdChanged)

        Q_PROPERTY(Department* department READ department WRITE setDepartment NOTIFY departmentChanged)

        X_TABLE_NAME("members")
        X_TABLE_FIELDS(X_FIELD(guid, X_PRIMARY_KEY),
                    X_FIELD(name),
                    X_FIELD(job),
                    X_FIELD(departmentId, "department_id", X_FOREIGN_KEY(Department, X_CASCADE, X_CASCADE)))

        X_FOREIGN_OBJECTS(X_FOREIGN_OBJECT(Department, guid, departmentId, department))

    public:
        explicit Member(QObject *parent = nullptr);
        Member(const quint64 &guid, const QString &name, const QString &job, const quint64 &departmentId,
            QObject *parent = nullptr);
        Member(const QString &name, const QString &job, const quint64 &departmentId, QObject *parent = nullptr);
        Member(const Member &other);

        ~Member();

        Member& operator=(const Member &other);

        quint64 guid() const;
        QString name() const;
        QString job() const;    
        quint64 departmentId() const;

        Department* department() const;

    public slots:
        void setGuid(const quint64 &guid);
        void setName(const QString &name);
        void setJob(const QString &job);
        void setDepartmentId(const quint64 &departmentId);

        void setDepartment(Department* department);

    signals:
        void guidChanged(const quint64 &guid);
        void nameChanged(const QString &name);
        void jobChanged(const QString &job);
        void departmentIdChanged(const quint64 &departmentId);

        void departmentChanged(Department* department);

    private:
        quint64 m_guid;
        QString m_name;
        QString m_job;
        quint64 m_departmentId;

        Department* m_department;

        void clearDepartment();
    };

#### One to Many
<img src="https://github.com/CamiloDelReal/x_orm/blob/develop/design/one_to_many.jpg" width="80%" height="80%" />

    class Project : public QObject, public XTableModel<Project>
    {
        Q_OBJECT

        Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
        Q_PROPERTY(quint64 memberId READ memberId WRITE setMemberId NOTIFY memberIdChanged)

        X_TABLE_NAME("projects")
        X_TABLE_FIELDS(X_FIELD(guid, X_PRIMARY_KEY),
                    X_FIELD(name),
                    X_FIELD(description),
                    X_FIELD(memberId, "member_id", X_FOREIGN_KEY(Member, X_CASCADE, X_CASCADE)))

    public:
        explicit Project(QObject *parent = nullptr);
        Project(const QString &name, const QString &description, QObject *parent = nullptr);
        Project(const QString &name, const QString &description, const quint64 &memberId, QObject *parent = nullptr);
        Project(const quint64 &guid, const QString &name, const QString &description, const quint64 &memberId, QObject *parent = nullptr);
        Project(const Project &other);

        Project& operator=(const Project &other);

        quint64 guid() const;
        QString name() const;
        QString description() const;
        quint64 memberId() const;

    public slots:
        void setGuid(const quint64 &guid);
        void setName(const QString &name);
        void setDescription(const QString &description);
        void setMemberId(const quint64 &memberId);

    signals:
        void guidChanged(const quint64 &guid);
        void nameChanged(const QString &name);
        void descriptionChanged(const QString &description);
        void memberIdChanged(const quint64 &memberId);

    private:
        quint64 m_guid;
        QString m_name;
        QString m_description;
        quint64 m_memberId;
    };

    class Member : public QObject, public XTableModel<Member>
    {
        Q_OBJECT

        Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString job READ job WRITE setJob NOTIFY jobChanged)

        Q_PROPERTY(QList<Project*> projects READ projects WRITE setProjects NOTIFY projectsChanged)

        X_TABLE_NAME("members")
        X_TABLE_FIELDS(X_FIELD(guid, X_PRIMARY_KEY),
                    X_FIELD(name),
                    X_FIELD(job))
        X_FOREIGN_LISTS(X_FOREIGN_LIST(Project, memberId, guid, projects))

    public:
        explicit Member(QObject *parent = nullptr);
        Member(const quint64 &guid, const QString &name, const QString &job, QObject *parent = nullptr);
        Member(const QString &name, const QString &job, QObject *parent = nullptr);
        Member(const Member &other);

        ~Member();

        Member& operator=(const Member &other);

        quint64 guid() const;
        QString name() const;
        QString job() const;

        QList<Project*> projects() const;

    public slots:
        void setGuid(const quint64 &guid);
        void setName(const QString &name);
        void setJob(const QString &job);

        void setProjects(QList<Project*> projects);

    signals:
        void guidChanged(const quint64 &guid);
        void nameChanged(const QString &name);
        void jobChanged(const QString &job);

        void projectsChanged(QList<Project*> projects);

    private:
        quint64 m_guid;
        QString m_name;
        QString m_job;

        QList<Project*> m_projects;

        void clearProjects();
    };

#### Many to Many
<img src="https://github.com/CamiloDelReal/x_orm/blob/develop/design/many_to_many.jpg" width="80%" height="80%" />

    class Project : public QObject, public XTableModel<Project>
    {
        Q_OBJECT

        Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

        Q_PROPERTY(QList<Member*> members READ members WRITE setMembers NOTIFY membersChanged)

        X_TABLE_NAME("projects")
        X_TABLE_FIELDS(X_FIELD(guid, X_PRIMARY_KEY),
                    X_FIELD(name),
                    X_FIELD(description))

        X_FOREIGN_LISTS(X_INTERMEDIATED_FOREIGN_LIST(Member, guid, Project, guid, MemberProject, memberId, projectId, members))

    public:
        explicit Project(QObject *parent = nullptr);
        Project(const QString &name, const QString &description, QObject *parent = nullptr);
        Project(const quint64 &guid, const QString &name, const QString &description, QObject *parent = nullptr);
        Project(const Project &other);

        ~Project();

        Project& operator=(const Project &other);

        quint64 guid() const;
        QString name() const;
        QString description() const;

        QList<Member*> members() const;

    public slots:
        void setGuid(const quint64 &guid);
        void setName(const QString &name);
        void setDescription(const QString &description);

        void setMembers(QList<Member*> members);

    signals:
        void guidChanged(const quint64 &guid);
        void nameChanged(const QString &name);
        void descriptionChanged(const QString &description);

        void membersChanged(QList<Member*> members);

    private:
        quint64 m_guid;
        QString m_name;
        QString m_description;

        QList<Member*> m_members;

        void clearMembers();
    };

    class Member : public QObject, public XTableModel<Member>
    {
        Q_OBJECT

        Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString job READ job WRITE setJob NOTIFY jobChanged)

        Q_PROPERTY(QList<Project*> projects READ projects WRITE setProjects NOTIFY projectsChanged)

        X_TABLE_NAME("members")
        X_TABLE_FIELDS(X_FIELD(guid, X_PRIMARY_KEY),
                    X_FIELD(name),
                    X_FIELD(job))
        X_FOREIGN_LISTS(X_INTERMEDIATED_FOREIGN_LIST(Project, guid, Member, guid, MemberProject, projectId, memberId, projects))

    public:
        explicit Member(QObject *parent = nullptr);
        Member(const quint64 &guid, const QString &name, const QString &job, QObject *parent = nullptr);
        Member(const QString &name, const QString &job, QObject *parent = nullptr);
        Member(const Member &other);

        ~Member();

        Member& operator=(const Member &other);

        quint64 guid() const;
        QString name() const;
        QString job() const;

        QList<Project*> projects() const;

    public slots:
        void setGuid(const quint64 &guid);
        void setName(const QString &name);
        void setJob(const QString &job);

        void setProjects(QList<Project*> projects);

    signals:
        void guidChanged(const quint64 &guid);
        void nameChanged(const QString &name);
        void jobChanged(const QString &job);

        void projectsChanged(QList<Project*> projects);

    private:
        quint64 m_guid;
        QString m_name;
        QString m_job;
        QList<Project*> m_projects;

        void clearProjects();
    };

    class MemberProject : public QObject, public XTableModel<MemberProject>
    {
        Q_OBJECT
        Q_PROPERTY(quint64 guid READ guid WRITE setGuid NOTIFY guidChanged)
        Q_PROPERTY(quint64 memberId READ memberId WRITE setMemberId NOTIFY memberIdChanged)
        Q_PROPERTY(quint64 projectId READ projectId WRITE setProjectId NOTIFY projectIdChanged)

        Q_PROPERTY(Member* member READ member WRITE setMember NOTIFY memberChanged)
        Q_PROPERTY(Project* project READ project WRITE setProject NOTIFY projectChanged)

        X_TABLE_NAME("members_projects")
        X_TABLE_FIELDS(X_FIELD(guid, X_PRIMARY_KEY),
                    X_FIELD(memberId, "member_id", X_FOREIGN_KEY(Member, X_CASCADE, X_CASCADE)),
                    X_FIELD(projectId, "project_id", X_FOREIGN_KEY(Project, X_CASCADE, X_CASCADE)))

    public:
        explicit MemberProject(QObject *parent = nullptr);
        MemberProject(const quint64 &memberId, const quint64 &projectId, QObject *parent = nullptr);
        MemberProject(const quint64 &guid, const quint64 &memberId, const quint64 &projectId, QObject *parent = nullptr);
        MemberProject(const MemberProject &other);

        ~MemberProject();

        MemberProject& operator=(const MemberProject &other);

        quint64 guid() const;
        quint64 memberId() const;
        quint64 projectId() const;

        Member* member() const;
        Project* project() const;

    public slots:
        void setGuid(const quint64 &guid);
        void setMemberId(const quint64 &memberId);
        void setProjectId(const quint64 &projectId);

        void setMember(Member* member);
        void setProject(Project* project);

    signals:
        void guidChanged(const quint64 &guid);
        void memberIdChanged(const quint64 &memberId);
        void projectIdChanged(const quint64 &projectId);

        void memberChanged(Member* member);
        void projectChanged(Project* project);

    private:
        quint64 m_guid;
        quint64 m_memberId;
        quint64 m_projectId;

        Member* m_member;
        Project* m_project;

        void clearMember();
        void clearProject();
    };