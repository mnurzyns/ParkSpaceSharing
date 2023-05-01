#include "MainDatabase.hh"

namespace server::database {

MainDatabase::MainDatabase(
  std::shared_ptr<oatpp::orm::Executor> const& executor)
  : oatpp::orm::DbClient{ executor }
{
    auto migrations = std::set<std::string>{};
    for (auto const& migration :
         std::filesystem::directory_iterator{ MAIN_DATABASE_MIGRATIONS_PATH }) {
        migrations.insert(migration.path().string());
    }

    oatpp::orm::SchemaMigration migrator{ executor };
    for (auto i = 0L; auto const& migration : migrations) { // NOLINT
        migrator.addFile(++i, migration);
        OATPP_LOGD("Database", "\tAdded migration %s", migration.c_str())
    }

    migrator.migrate();
    OATPP_LOGI("Database",
               "\tApplied migrations. Database version: %lld",
               executor->getSchemaVersion())
}

} // namespace server::database
