#include "MainDatabase.hh"

#include <set>

#include "Config.hh"

namespace server::database {

MainDatabase::MainDatabase(
  std::shared_ptr<oatpp::orm::Executor> const& executor)
  : oatpp::orm::DbClient{ executor }
{
    std::string_view migrations_path =
      server::Config::getInstance().database_migrations_path;
    if (!std::filesystem::exists(migrations_path)) {
        migrations_path = MAIN_DATABASE_MIGRATIONS_PATH;
        if (!std::filesystem::exists(migrations_path)) {
            throw std::runtime_error{
                "server.database.migrations_path does not point to a valid "
                "directory. Check your configuration."
            };
        }
    }

    OATPP_LOGI("Database",
               "\tUsing \"%*s\" as migrations path.",
               migrations_path.length(),
               migrations_path.data())

    auto migrations = std::set<std::string>{};
    for (auto const& migration :
         std::filesystem::directory_iterator{ migrations_path }) {
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
