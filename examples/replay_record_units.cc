#include "unit_spatial_map.h"
#include "sc2api/sc2_api.h"
#include "sc2api/sc2_score.h"


#include "sc2utils/sc2_manage_process.h"

#include <iostream>
#include <fstream>
#include <filesystem>

const char* kReplayFolder = "C:\\Program Files (x86)\\StarCraft II\\Replays\\";

using namespace sc2;
using namespace std;

string raceToString(Race race)
{
    switch (race)
    {
    case   Terran: return "Terran";
    case Zerg: return "Zerg";
    case Protoss: return "Protoss";
    case Random: return "Random";
    }
    return "";
}

string unitTypeToName(UnitTypeID uid)
{
    switch (uid.ToType())
    {
    case UNIT_TYPEID::TERRAN_STARPORTREACTOR:  return "TERRAN_STARPORT REACTOR";
    case UNIT_TYPEID::TERRAN_STARPORTTECHLAB:  return "TERRAN_STARPORT TECHLAB";
    case UNIT_TYPEID::TERRAN_BARRACKSREACTOR:  return "TERRAN_BARRACKS REACTOR";
    case UNIT_TYPEID::TERRAN_BARRACKSTECHLAB:  return "TERRAN_BARRACKS TECHLAB";
    case UNIT_TYPEID::TERRAN_FACTORYREACTOR:   return "TERRAN_FACTORY REACTOR";
    case UNIT_TYPEID::TERRAN_FACTORYTECHLAB:   return "TERRAN_FACTORY TECHLAB";
    }
    return UnitTypeToName(uid);
}

class RecordUnits : public sc2::ReplayObserver {
public:
    RecordUnits(ostream* out) : fout(out), sc2::ReplayObserver()
    {
        iReportThisStep = false;
        nGames = 0;
    }

    ~RecordUnits() { }

    virtual void reportUnits() = 0;

    void OnGameStart() override {
        const sc2::ObservationInterface* obs = Observation();
        *fout << "NewGame\n" << Observation()->GetGameInfo().map_name;
        cout << "\nGame " << ++nGames << ": " << Observation()->GetGameInfo().map_name;
    }

    void OnUnitCreated(const sc2::Unit* unit) override {
        iReportThisStep = true;
    }

    void OnUnitEnterVision(const sc2::Unit* u) override {
        iReportThisStep = true;
    }

    void OnUnitDestroyed(const sc2::Unit* u) override {
        iReportThisStep = true;
    }

    void OnUnitsDamaged(Units const& u) override
    {
    }

    void OnStep() override
    {
        if (iReportThisStep)
        {
            reportUnits();
            iReportThisStep = false;
        }
    }

    void RecordUnits::OnGameEnd() override {
        Score const& s = Observation()->GetScore();
        auto& sd = s.score_details;
        *fout << "\nGameOver" << ' ' << '.' << std::endl;
        iReportThisStep = false;
    }

protected:
    std::ostream* fout;
    bool iReportThisStep;
    int  nGames;
};

class RecordVisibleUnits : public RecordUnits {
public:
    RecordVisibleUnits(ofstream* out) : RecordUnits(out)
    {
    }

    void reportUnits() override
    {

        char delim = ' ';
        size_t frame = Observation()->GetGameLoop() / (16 * 30);

        UnitTypes const& ud = Observation()->GetUnitTypeData();
        auto us = Observation()->GetUnits(); // get all known units
        UnitSpatialMap map(us.begin(), us.end());

        if (!iNewUnits.empty())
        {
            *fout << '\n' << frame << delim << "Created";
            for (auto const* u : iNewUnits)
            {
                UnitTypeData const& d = ud[(int)u->unit_type];
                float sr2 = d.sight_range * d.sight_range;
                auto [a, b] = map.getUnitsNear(u, max(sr2, 0.01f));
                if (a == b)
                    continue;
                *fout << delim << unitTypeToName((*a)->unit_type);
                for (++a; a != b; ++a)
                {
                    *fout << delim << unitTypeToName((*a)->unit_type);
                }
            }
            iNewUnits.clear();
        }

        if (!iEnteredVisionUnits.empty())
        {
            *fout << '\n' << frame << delim << "EnteredVision";
            for (auto const* u : iEnteredVisionUnits)
            {
                UnitTypeData const& d = ud[(int)u->unit_type];
                float sr2 = d.sight_range * d.sight_range;
                auto [a, b] = map.getUnitsNear(u, max(sr2, 0.01f));
                if (a == b)
                    continue;
                *fout << delim << unitTypeToName((*a)->unit_type);
                for (++a; a != b; ++a)
                {
                    *fout << delim << unitTypeToName((*a)->unit_type);
                }
            }
            iEnteredVisionUnits.clear();
        }

        if (!iLostUnits.empty())
        {
            *fout << '\n' << frame << delim << "Destroyed";
            for (auto const* u : iLostUnits)
            {
                UnitTypeData const& d = ud[(int)u->unit_type];
                float sr2 = d.sight_range * d.sight_range;
                auto [a, b] = map.getUnitsNear(u, max(sr2, 0.01f));
                if (a == b)
                    continue;
                *fout << delim << unitTypeToName((*a)->unit_type);
                for (++a; a != b; ++a)
                {
                    *fout << delim << unitTypeToName((*a)->unit_type);
                }
            }
            iLostUnits.clear();
        }

    }

    void OnUnitCreated(const sc2::Unit* unit) {
        RecordUnits::OnUnitCreated(unit);
        iNewUnits.push_back(unit);
    }

    void OnUnitEnterVision(const sc2::Unit* u) {
        RecordUnits::OnUnitEnterVision(u);
        iEnteredVisionUnits.push_back(u);
    }

    void OnUnitDestroyed(const sc2::Unit* u) {
        RecordUnits::OnUnitDestroyed(u);
        iLostUnits.push_back(u);
    }

    Units iNewUnits;
    Units iLostUnits;
    Units iEnteredVisionUnits;
};

class RecordVisibleUnits2 : public RecordUnits {
public:
    RecordVisibleUnits2(ofstream* out) : RecordUnits(out)
    {
    }

    void reportUnits() override
    {

        char delim = ' ';
        size_t frame = Observation()->GetGameLoop() / (16 * 30);

        UnitTypes const& ud = Observation()->GetUnitTypeData();
        auto us = Observation()->GetUnits(); // get all known units
        UnitSpatialMap map(us.begin(), us.end());

        if (!iNewUnits.empty())
        {
            *fout << '\n' << frame << delim << "Created";
            for (auto const* u : iNewUnits)
            {
                UnitTypeData const& d = ud[(int)u->unit_type];
                float sr2 = d.sight_range * d.sight_range;
                auto [a, b] = map.getUnitsNear(u, max(sr2, 0.01f));
                if (a == b)
                    continue;
                *fout << delim << unitTypeToName((*a)->unit_type);
                for (++a; a != b; ++a)
                {
                    *fout << delim << unitTypeToName((*a)->unit_type);
                }
            }
            iNewUnits.clear();
        }

        if (!iEnteredVisionUnits.empty())
        {
            *fout << '\n' << frame << delim << "EnteredVision";
            for (auto const* u : iEnteredVisionUnits)
            {
                UnitTypeData const& d = ud[(int)u->unit_type];
                float sr2 = d.sight_range * d.sight_range;
                auto [a, b] = map.getUnitsNear(u, max(sr2, 0.01f));
                if (a == b)
                    continue;
                *fout << delim << unitTypeToName((*a)->unit_type);
                for (++a; a != b; ++a)
                {
                    *fout << delim << unitTypeToName((*a)->unit_type);
                }
            }
            iEnteredVisionUnits.clear();
        }

        if (!iLostUnits.empty())
        {
            *fout << '\n' << frame << delim << "Destroyed";
            for (auto const* u : iLostUnits)
            {
                UnitTypeData const& d = ud[(int)u->unit_type];
                float sr2 = d.sight_range * d.sight_range;
                auto [a, b] = map.getUnitsNear(u, max(sr2, 0.01f));
                if (a == b)
                    continue;
                *fout << delim << unitTypeToName((*a)->unit_type);
                for (++a; a != b; ++a)
                {
                    *fout << delim << unitTypeToName((*a)->unit_type);
                }
            }
            iLostUnits.clear();
        }

    }

    void OnUnitCreated(const sc2::Unit* unit) {
        RecordUnits::OnUnitCreated(unit);
        iNewUnits.push_back(unit);
    }

    void OnUnitEnterVision(const sc2::Unit* u) {
        RecordUnits::OnUnitEnterVision(u);
        iEnteredVisionUnits.push_back(u);
    }

    void OnUnitDestroyed(const sc2::Unit* u) {
        RecordUnits::OnUnitDestroyed(u);
        iLostUnits.push_back(u);
    }

    Units iNewUnits;
    Units iLostUnits;
    Units iEnteredVisionUnits;
};

class RecordAllUnits : public RecordUnits {
    RecordAllUnits(ofstream* out) : RecordUnits(out)
    {
        Debug()->DebugShowMap(); // hopefully make alles visible
    }

    void reportUnits() override
    {
        auto us = Observation()->GetUnits(Unit::Alliance::Self);
        auto them = Observation()->GetUnits(Unit::Alliance::Enemy);

        char delim = ' ';
        size_t frame = Observation()->GetGameLoop() / (16 * 30);
        auto& d = Observation()->GetUnitTypeData();
        *fout << "\nSelf" << delim << frame
            << delim << raceToString(d[us.front()->unit_type].race);
        for (auto u : us)
            * fout << delim << unitTypeToName(u->unit_type);
        *fout << delim << '.';

        *fout << "\nEnemy" << delim << frame
            << delim << raceToString(d[them.front()->unit_type].race);
        for (auto u : them)
            * fout << delim << unitTypeToName(u->unit_type);
        *fout << delim << '.';

    }
};

bool hasOrder(ObservationInterface const* obs, const Unit* u, const AbilityID aid)
{
    auto gaid = GetGeneralizedAbilityID(aid, *obs);
    return any_of(u->orders.begin(), u->orders.end(),
        [&](auto& a) { return a.ability_id == gaid; }); // GetGeneralizedAbilityID has already been called for units (if UseGeneralizedAbility is set). So it is not really needed here
}

class RecordChanges : public RecordVisibleUnits {
public:
    RecordChanges(ofstream* out) : RecordVisibleUnits(out)
    {
    }

    virtual void reportUnits() override {

        if (!iNewUnits.empty())
        {
            for (auto const* u : iNewUnits)
                * fout << '\n' << "Created" << iDelim << unitTypeToName(u->unit_type);
            iNewUnits.clear();
        }

        if (!iEnteredVisionUnits.empty())
        {
            for (auto const* u : iEnteredVisionUnits)
                * fout << '\n' << "Observed" << iDelim << unitTypeToName(u->unit_type);
            iEnteredVisionUnits.clear();
        }

        if (!iLostUnits.empty())
        {
            for (auto const* u : iLostUnits)
                * fout << '\n' << "Destroyed" << iDelim << unitTypeToName(u->unit_type);
            iLostUnits.clear();
        }
    }

    virtual void reportAttack(const Unit* u) {
        auto const* enemy = Observation()->GetUnit(u->engaged_target_tag);
        if (enemy) // insisting on having an engaged target target will prevent reporting when under attack move commands
        {
            *fout << '\n' << unitTypeToName(u->unit_type)
                << iDelim << "engaged"
                << iDelim << unitTypeToName(enemy->unit_type);
        }
    }

    void OnStep() override
    {
        RecordVisibleUnits::OnStep();
        auto* obs = Observation();
        auto isAttacking = [obs](Unit const& u) { return hasOrder(obs, &u, ABILITY_ID::ATTACK); };
        auto us = obs->GetUnits(isAttacking);

        for (auto u : us)
        {
            reportAttack(u);
        }
    }

    char iDelim = ' ';


};

int main(int argc, char* argv[]) {
    sc2::Coordinator coordinator;
    if (!coordinator.LoadSettings(argc, argv)) {
        return 1;
    }

    if (!coordinator.SetReplayPath(kReplayFolder)) {
        std::cout << "Unable to find replays." << std::endl;
        return 1;
    }

    std::string base_dir = ".";
    std::filesystem::path path = base_dir;
    path /= "logs";
    std::filesystem::create_directories(path);

    std::filesystem::path fname = path / "replay_history_nearby_units2.txt";
    ofstream fout(fname);
    bool l = fout.is_open();

    RecordVisibleUnits replay_observer(&fout);
    coordinator.AddReplayObserver(&replay_observer);

    while (coordinator.Update());
}

