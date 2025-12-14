#ifndef IAPP_MODULE_H
#define IAPP_MODULE_H

#include "hw/gfx_conf.h"

/**
 * @brief Rozhraní, které musí implementovat každá hra/aplikace, 
 * která má být zobrazena jako ikona na MainScreen.
 */
class IAppModule {
public:
    virtual ~IAppModule() = default;

    /**
     * @brief Vrací text, který se zobrazí pod ikonou.
     */
    virtual const char* getName() const = 0;

    /**
     * @brief Vrací cestu k souboru ikony (např. PNG/BMP) na SD kartě.
     * Pro jednoduchost můžeme začít s prostým textem.
     */
    virtual const char* getIconPath() const = 0; 
    
    /**
     * @brief Spustí hru/modul. Volá se po stisknutí ikony.
     * Měla by se postarat o vykreslení herní obrazovky.
     */
    virtual void run() = 0;
    
    /**
     * @brief Kontroluje, zda je modul aktivní (běží). 
     * Užitečné pro kontrolu v hlavním loopu.
     */
    virtual bool isActive() const = 0;
};

#endif // IGAME_MODULE_HPP