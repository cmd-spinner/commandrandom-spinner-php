import { IFilter, Entry } from '../types';
import { getApplicationPid } from './adb';
import { Priority } from './constants';

type Filter = (entry: Entry) => boolean;

export default class AndroidFilter implements IFilter {
  private readonly minPriority: number;
  private filter: Filter;

  constructor(minPriority: number = 0) {
    this.minPriority = minPriority;
    // Default filter by all
    this.filter = (entry: Entry) => {
      return entry.priority >= this.minPriority;
    };
  }

  setFilterByTag(tags: string[]) {
    this.filter = (entry: Entry) => {
      return Boolean(
        entry.priority >= this.minPriority &&
          entry.tag &&
          tags.indexOf(entry.tag) > -1
      );
    };
  }

  setFilterByApp(applicationId: string, adbPath?: string) {
    const pid = getApplicationPid(applicationId, adbPath);
    this.filter = (entry: Entry) => {
      return entry.priority >= this.minPriority && entry.pid === pid;
    };
  }

  setFilterByMatch(regexes: RegExp[]) {
    this.filter = (entry: Entry) => {
      return (
        entry.priority >= this.minPriority &&
        Boolean(
          regexes.find((reg: RegExp) =>
            Boolean(entry.messages.find((message: string) => reg.test(message)))
          )
        )
      );
    };
  }

  setCustomFilter(patterns: string[]) {
    const tagFilters: { [key: string]: number } = patterns.reduce(
      (acc: { [key: string]: number }, pattern: string) => {
        const [tag, priority] = pattern.split(':');
        return {
          ...acc,
          [tag]: Priority.fromLetter(priority),
        };
      },
      {}
    );
    this.filter = (entry: Entry) => {
      return (
        (entry.tag &&
          entry.priority >= (tagFilters[entry.tag] || Priority.SILENT)) ||
        entry.priority >= (tagFilters['*'] || Priority.UNKNOWN)
      );
    };
  }

  shouldInclude(entry: Entry) {
    return this.filter(entry);
  }
}
